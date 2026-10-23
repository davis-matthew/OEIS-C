/*
 * A124936  Numbers k such that k - 1 and k + 1 are semiprimes.
 * https://oeis.org/A124936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that k−1 and k+1 are semiprimes.
 *            The function searches forward from k = 2, using a small sieve of
 *            smallest prime factors (spf) to test semiprimality. No
 *            memoisation is used; each call recomputes the needed terms. This
 *            is fast enough for n ≤ 10000 on the allowed time...
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* the function requested by the build system */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Global data: smallest prime factor array                          */
/* ------------------------------------------------------------------ */
static uint32_t *spf      = NULL;   /* spf[i] == smallest prime factor of i (i>=2) */
static size_t   spf_size  = 0;      /* number of elements in spf (indices 0..spf_size-1) */

/* ------------------------------------------------------------------ */
/*  Build the sieve of smallest prime factors up to 'limit' (inclusive)   */
/*  The array is rebuilt from scratch each time a larger limit is needed.    */
/*  Allocation uses allocators that are compatible with C++ arbitrary-      */
/*  precision integer classes (they are merely raw memory allocations).   */
/* ------------------------------------------------------------------ */
static void
ensure_sieve_limit(idx_t limit)
{
    if (limit < 2) return;                   /* nothing to do */
    if (spf && spf_size > limit) return;     /* already sufficient */

    size_t n = (size_t)limit + 1;            /* size of array needed */
    uint32_t *new_spf = (uint32_t*)calloc(n, sizeof(uint32_t));
    if (!new_spf) return;                    /* allocation failed – unlikely */

    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!new_spf[i]) {                   /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= (size_t)limit; j += i)
                    if (!new_spf[j]) new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);   /* free previous allocation */
    spf = new_spf;
    spf_size = n;
}

/* ------------------------------------------------------------------ */
/*  Test whether a number n (>=2) is a semiprime (product of two primes). */
/*  Uses the global 'spf' array.                                         */
/* ------------------------------------------------------------------ */
static inline bool
is_semiprime(idx_t n)
{
    if (n < 4) return false;                 /* smallest semiprime is 4 */

    if (spf_size-1 < (size_t)n) ensure_sieve_limit(n);

    uint32_t p = spf[n];
    if (!p) return false;                    /* should not happen */

    idx_t q = n / (idx_t)p;
    return (q >= 2) && (spf[(size_t)q] == (uint32_t)q);   /* q must be prime */
}

/* ------------------------------------------------------------------ */
/*  Main public function: return the nth term of the sequence.        */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1 – return 0 for n=0 */

    idx_t count = 0;
    idx_t k = 2;                             /* start searching from k=2 */

    while (count < n) {
        if (k + 1 >= spf_size) ensure_sieve_limit(k + 1);

        if (is_semiprime(k - 1) && is_semiprime(k + 1)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }

    /* This line should never be reached. */
    return (seq_t)0;
}
