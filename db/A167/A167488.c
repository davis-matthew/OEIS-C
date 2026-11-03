/*
 * A167488  Primes p such that (p reversed)-6 is also a prime.
 * https://oeis.org/A167488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that reverse(p) – 6 is also prime (OEIS A167488) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/reverse_digits.h"
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Cached qualifying primes */
static seq_t *qual_primes = NULL;   /* array of qualifying primes   */
static size_t  qual_cap   = 0;      /* current array capacity       */
static size_t  qual_len   = 0;      /* number of stored primes      */
static seq_t  next_prime_candidate = 2; /* next integer to test       */

/* Ensure that the cache contains at least 'num' qualifying primes */
static void ensure_cache(idx_t num)
{
    /* Initialise cache on first use */
    if (qual_primes == NULL) {
        qual_cap   = 16;
        qual_primes = seq_alloc_array(qual_cap);
        qual_len   = 0;
        next_prime_candidate = 2;
    }

    while (qual_len < (size_t)num) {
        seq_t cand = next_prime_candidate++;
        /* Skip non‑primes */
        if (!is_prime_big(cand))
            continue;

        seq_t rev = reverse_digits(cand);
        /* rev must be > 6 so that rev - 6 is non‑negative */
        if (rev <= 6)
            continue;

        seq_t cand2 = rev - 6;
        if (!is_prime_big(cand2))
            continue;

        /* Grow the array if needed */
        if (qual_len == qual_cap) {
            size_t new_cap = qual_cap << 1;          /* double */
            seq_t *new_arr = seq_alloc_array(new_cap);
            for (size_t i = 0; i < qual_len; ++i)
                new_arr[i] = qual_primes[i];
            seq_free_array(qual_primes);
            qual_primes = new_arr;
            qual_cap   = new_cap;
        }

        /* Store qualifying prime */
        qual_primes[qual_len++] = cand;
    }
}

/* External function required by the testing harness */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – undefined for n = 0 */
        return (seq_t)0;
    ensure_cache(n);
    return qual_primes[n - 1];
}
