/*
 * A089783  Smallest prime(k) pertaining to A089782.
 * https://oeis.org/A089783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime(k) such that the concatenation of prime(k)
 *            and prime(k+n) is itself prime
 * Range:     u8 n <= 2, u16 n <= 57, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities provided by the build */
#include "../utils/is_prime.h"
#include "../utils/num_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/*  Prime list handling                                                 */
/* --------------------------------------------------------------------- */
static uint64_t *prime_buf = NULL;     /* 0‑indexed array of primes        */
static size_t    prime_cnt = 0;        /* number of primes stored          */
static size_t    prime_cap = 0;        /* current capacity of prime_buf    */
static uint64_t  next_candidate = 2;   /* next integer to test for primality */

/* Ensure prime_buf contains at least 'need' primes (1‑based count).   */
static void
ensure_primes(size_t need)
{
    /* Grow capacity if required. */
    if (prime_cap < need) {
        size_t new_cap = prime_cap == 0 ? 64 : prime_cap * 2;
        if (new_cap < need) new_cap = need;
        uint64_t *new_buf = (uint64_t *)realloc(prime_buf, new_cap * sizeof(uint64_t));
        if (!new_buf) abort();
        prime_buf = new_buf;
        prime_cap = new_cap;
    }

    /* Append primes until we have 'need' of them. */
    while (prime_cnt < need) {
        /* Skip even numbers after 2. */
        if (next_candidate == 2) {
            /* 2 is prime. */
            prime_buf[prime_cnt++] = next_candidate;
            next_candidate = 3;
            continue;
        }
        /* Find next prime by trial test. */
        while (!is_prime(next_candidate))
            next_candidate += 2;
        prime_buf[prime_cnt++] = next_candidate;
        next_candidate += 2;
    }
}

/* Retrieve the k‑th prime (1‑based).  Caller guarantees k >= 1. */
static inline uint64_t
prime_at(idx_t k)
{
    ensure_primes((size_t)k);
    return prime_buf[k - 1];
}

/* --------------------------------------------------------------------- */
/*  OEIS sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* a(0) is undefined */

    /* Search for the smallest k such that the concatenation
       of prime(k) and prime(k+n) is prime. */
    for (idx_t k = 1; ; ++k) {
        idx_t required = k + n;          /* maximum prime index needed */
        ensure_primes((size_t)required);

        uint64_t p1 = prime_at(k);
        uint64_t p2 = prime_at(required);

        /* Compute the decimal concatenation of p1 and p2.          */
        seq_t factor = pow10((idx_t)num_digits_idx((idx_t)p2));
        seq_t concat = ((seq_t)p1) * factor + (seq_t)p2;

        if (is_prime_big(concat))
            return (seq_t)p1;            /* smallest such prime(k) found */
    }
}
