/*
 * A033640  Base 3 digital convolution sequence.
 * https://oeis.org/A033640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 1; for n > 0, let n = d_{m-1}…d_1 d_0 in base 3. Then
 *            a(n) = Σ_{k=0}^{m-1} d_{m-1-k} · a(n-1-k)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 43, u64 n <= 79, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recursive definition directly, memoising all
   values in a static array so any order of calls is supported. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed values */
static size_t cap   = 0;         /* current allocation size (in elements) */
static idx_t  max_computed = 0;  /* largest index for which cache[i] is valid */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cap >= needed)
        return;
    size_t new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                  /* out of memory */
    cache = new_cache;
    cap = new_cap;
    /* initialise new elements to zero (optional, keeps debugging safe) */
    for (size_t i = max_computed + 1; i < cap; ++i)
        cache[i] = (seq_t)0;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Handle the base case explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* Ensure we have room for n. */
    ensure_capacity(n);
    size_t N = (size_t)n;

    /* Initialise cache[0] on first call. */
    if (!cache[0] && max_computed < 1) {
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* Compute any missing values up to N. */
    if (N > (size_t)max_computed) {
        /* Compute values in ascending order. */
        for (size_t i = max_computed + 1; i <= N; ++i) {
            idx_t t = (idx_t)i;
            int  digits[64];
            int  dlen = 0;

            /* Convert i to base‑3 digits (least significant first). */
            while (t > 0) {
                digits[dlen++] = (int)(t % 3);
                t /= 3;
            }
            if (dlen == 0)            /* i == 0, already handled */
                dlen = 1, digits[0] = 1;

            /* Compute convolution using the digits. */
            seq_t sum = 0;
            size_t start = i - (size_t)dlen;  /* index of a(i-dlen) */
            for (int k = 0; k < dlen; ++k) {
                seq_t coeff = (seq_t)digits[dlen - 1 - k];
                sum += coeff * cache[start + k];
            }
            cache[i] = sum;
        }
        max_computed = N;
    }

    return cache[N];
}
