/*
 * A068887  Primes with property that digits alternate in parity individually
 *          as well as in concatenation with previous terms.
 * https://oeis.org/A068887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A068887 – primes whose decimal digits alternate in parity and whose
 * most–significant digit’s parity differs from the least–significant digit of
 * the preceding term.
 *
 * Each term is a prime p such that
 *   (1) digits(p) alternate even/odd,
 *   (2) if a(n‑1) exists then parity( msd(p) ) != parity( lsd( a(n‑1) ) ).
 *
 * Implementation: incremental generation with memoization.
 *   • is_prime_big(seq_t) is used for primality testing.
 *   • digits_alternate() checks intra‑prime parity alternation.
 *   • msd_parity() and lsd_parity() obtain parities of extremal digits.
 *   • the sequence is stored in a static dynamic array, expanded only as
 *     needed.  Calls with arbitrary n are answered from cache, so the
 *     ordering of calls is irrelevant.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: true if decimal digits of x alternate in parity. */
static bool digits_alternate(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        int parity = d & 1;
        if (prev != -1 && parity == prev) return false;
        prev = parity;
        x = x / 10;
    }
    return true;
}

/* Return parity of least significant decimal digit of x. */
static int lsd_parity(seq_t x)
{
    return (int)(x % 10) & 1;
}

/* Return parity (0 even, 1 odd) of the most significant decimal digit of x. */
static int msd_parity(seq_t x)
{
    while (x >= 10) x = x / 10;
    return (int)(x % 10) & 1;
}

/* Return the n‑th term of sequence A068887.  N is 1‑based. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Expand cache until it contains n terms. */
    while (cache_len < n) {
        seq_t candidate;

        /* Start with the next integer after the last term. */
        if (cache_len == 0) {
            candidate = 2;
        } else {
            candidate = cache[cache_len - 1] + 1;
        }

        /* Search for the next admissible prime. */
        while (true) {
            /* Skip even numbers after the first prime. */
            if (candidate != 2 && (candidate & 1) == 0) {
                candidate = candidate + 1;
                continue;
            }

            /* 1) Must be prime. */
            if (!is_prime_big(candidate)) {
                candidate = candidate + 2;
                continue;
            }

            /* 2) Digits must alternate in parity. */
            if (!digits_alternate(candidate)) {
                candidate = candidate + 2;
                continue;
            }

            /* 3) Boundary parity must alternate with the previous term. */
            if (cache_len > 0 &&
                lsd_parity(cache[cache_len - 1]) == msd_parity(candidate)) {
                candidate = candidate + 2;
                continue;
            }

            /* Found a valid term. Store it. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
            break;      /* exit inner search loop */
        }
    }

    /* Convert the 1‑based index to 0‑based array index. */
    return cache[n - 1];
}
