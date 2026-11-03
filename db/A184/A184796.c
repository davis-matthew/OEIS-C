/*
 * A184796  Primes of the form floor(k*sqrt(3)).
 * https://oeis.org/A184796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the Beatty sequence floor(k*sqrt(3))
 *            (OEIS A184796). This implementation avoids floating point by
 *            computing floor(k*sqrt(3)) as floor_sqrt(3*k^2). The primes are
 *            tested using is_prime_big and cached for subsequent calls
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------
   Include needed utilities
  ------------------------------*/
#include <stdlib.h>          /* malloc, realloc, free */
#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool, true, false */
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

/* -------------------------------
   Compute the n-th term
   -------------------------------*/
seq_t a(idx_t n)
{
    /* n >= 1 for valid queries; return 0 for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found primes */
    static seq_t  *primes    = NULL;    /* array of primes */
    static size_t primes_len = 0;       /* number of primes stored */
    static size_t primes_cap = 0;       /* allocated capacity */
    static idx_t  k_last    = 0;       /* last k examined */

    /* Initialize cache on first call */
    if (primes == NULL) {
        primes_cap = 16;
        primes     = mem_new(seq_t, primes_cap);
        if (!primes) return (seq_t)0;  /* allocation failure */
    }

    /* Generate primes until we have at least n of them */
    while (primes_len < (size_t)n) {
        /* Find next candidate k */
        do {
            ++k_last;                                 /* next k */
            seq_t k  = (seq_t)k_last;
            seq_t k2 = k * k;
            seq_t t  = k2 * (seq_t)3;                 /* 3*k^2 */
            seq_t val = floor_sqrt(t);                /* floor(k*sqrt(3)) */

            if (is_prime_big(val)) {
                /* Grow array if needed */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                    if (!tmp) return (seq_t)0;        /* realloc failed */
                    primes    = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = val;            /* store prime */
                break;                                 /* found one */
            }
        } while (true);
    }

    return primes[n - 1];
}
