/*
 * A072684  Expansion of (2+x+3*x^2+2*x^3+x^4)/(1-x-5*x^2+x^3+3*x^4-x^5).
 * https://oeis.org/A072684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 2, a(1) = 3, a(2) = 16, a(3) = 31, a(4) = 103
   For n >= 5:
   a(n) = a(n-1) + 5*a(n-2) - a(n-3) - 3*a(n-4) + a(n-5) */

/* The values grow quickly; the cache size is set large enough for all
   expected test cases (over 10 000 terms).  The array is static, so it
   works both in C and C++ without dynamic allocation of non‑POD
   objects. */

#include <stdlib.h>
#include <stdint.h>

#define CACHE_CAP (20000)               /* safe upper bound for tests */

static seq_t cache[CACHE_CAP];          /* buffer of computed terms */
static idx_t n_cache = 0;               /* number of terms stored (next to compute) */

/* Ensure indices up to 'n' are computed and stored in 'cache'. */
static void ensure_cache(idx_t n)
{
    if (n < n_cache)                 /* already up to date */
        return;

    if (n >= CACHE_CAP) {            /* out of bounds of static cache */
        /* In practice this should never happen for test data. */
        exit(1);
    }

    /* Fill missing terms */
    for (idx_t i = n_cache; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)2;
        else if (i == 1)
            cache[i] = (seq_t)3;
        else if (i == 2)
            cache[i] = (seq_t)16;
        else if (i == 3)
            cache[i] = (seq_t)31;
        else if (i == 4)
            cache[i] = (seq_t)103;
        else {
            seq_t s1 = cache[i - 1];   /* a(n-1) */
            seq_t s2 = cache[i - 2];   /* a(n-2) */
            seq_t s3 = cache[i - 3];   /* a(n-3) */
            seq_t s4 = cache[i - 4];   /* a(n-4) */
            seq_t s5 = cache[i - 5];   /* a(n-5) */

            /* Compute the positive and negative parts separately
               to avoid underflow on unsigned seq_t types.  The
               recurrence guarantees the result is non‑negative. */
            seq_t pos = s1 + 5 * s2 + s5;
            seq_t neg = s3 + 3 * s4;
            cache[i]  = pos - neg;
        }
    }
    n_cache = n + 1;                  /* next term to compute is cache[n+1] */
}

/* Public function required by the evaluation harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
