/*
 * A060605  a(n) = sum of lengths of the iteration sequences of Euler totient
 *          function from 1 to n.
 * https://oeis.org/A060605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1..n} A049108(j) (sum of lengths of the
 *            φ‑iteration)
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* internal memoisation (static, thread‑unsafe) */
static idx_t cached_n = 0;           /* largest n computed so far          */
static idx_t *phi_cache = NULL;      /* Euler φ of 0..n                     */
static seq_t *len_cache = NULL;      /* chain length of 0..n                */
static seq_t *sum_cache = NULL;      /* partial sums of lengths             */
static idx_t cache_cap = 0;          /* current capacity of the arrays      */

/* recompute all data up to (and including) n */
static void recompute(idx_t n)
{
    /* ------------------------------------------------------------ *
     *  Allocate / resize arrays big enough for indices 0..n.        *
     * ------------------------------------------------------------ */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (phi_cache)     phi_cache = (idx_t *)realloc(phi_cache, new_cap * sizeof(idx_t));
        else               phi_cache = (idx_t *)malloc(new_cap * sizeof(idx_t));
        if (!phi_cache) abort();

        if (len_cache)     len_cache = (seq_t *)realloc(len_cache, new_cap * sizeof(seq_t));
        else               len_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!len_cache) abort();

        if (sum_cache)     sum_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        else               sum_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!sum_cache) abort();

        cache_cap = (idx_t)(n + 1);
    }

    /* ------------------------------------------------------------ *
     *  Compute φ(0..n) by a simple sieve.                         *
     * ------------------------------------------------------------ */
    for (idx_t i = 0; i <= n; ++i) {
        phi_cache[i] = (idx_t)i;
    }
    for (idx_t p = 2; p <= n; ++p) {
        if (phi_cache[p] == p) {          /* p is prime               */
            for (idx_t j = p; j <= n; j += p) {
                phi_cache[j] -= phi_cache[j] / p;
            }
        }
    }

    /* ------------------------------------------------------------ *
     *  Chain lengths: len[1] = 1; len[i] = 1 + len[φ[i]] .         *
     * ------------------------------------------------------------ */
    len_cache[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        len_cache[i] = (seq_t)1 + len_cache[phi_cache[i]];
    }

    /* ------------------------------------------------------------ *
     *  Partial sums of the lengths.  sum_cache[0] = 0.             *
     * ------------------------------------------------------------ */
    sum_cache[0] = (seq_t)0;
    seq_t running_sum = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        running_sum += len_cache[i];
        sum_cache[i] = running_sum;
    }

    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1, below offset ignored */
    if (n > cached_n) recompute(n);
    return sum_cache[n];
}
