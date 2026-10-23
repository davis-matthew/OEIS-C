/*
 * A119802  a(1) = 1. For m >= 0 and 1 <= k <= 2^m, a(2^m +k) = number of
 *          earlier terms of the sequence which equal a(k).
 * https://oeis.org/A119802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n>1 let p = 2^⌊log₂(n‑1)⌋ and k = n‑p; then a(n) =
 *            number of indices j with 1 ≤ j < n for which a(j) = a(k)
 * Range:     u8 n <= 5718, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq      = NULL;   /* array of computed terms, 0‑based */
static idx_t  seq_len  = 0;      /* number of terms already computed */
static idx_t  seq_cap  = 0;      /* current allocated capacity */

/* integer floor‑log2 for a 64‑bit unsigned value (x>0) */
static idx_t floor_log2_u64(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1U)
        ++r;
    return r;
}

/* ensure there is room for at least new_cap elements */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure – terminate */
    seq     = tmp;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* sequence is defined from n=1 */

    /* first call: initialise array with a(1)=1 */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* compute missing terms up to n */
    if (n > seq_len) {
        ensure_capacity(n);
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            if (i == 1) {
                seq[0] = (seq_t)1;    /* safety; already set */
                continue;
            }
            /* find highest power of two < i */
            idx_t m   = floor_log2_u64(i - 1U);
            idx_t p   = ((idx_t)1U) << m;   /* 2^m */
            idx_t k   = i - p;              /* 1 ≤ k ≤ 2^m */
            seq_t val_k = seq[k - 1U];      /* a(k) */

            /* count occurrences of val_k among a(1)..a(i‑1) */
            seq_t cnt = (seq_t)0;
            for (idx_t j = 0U; j < i - 1U; ++j)
                if (seq[j] == val_k)
                    ++cnt;

            seq[i - 1U] = cnt;
        }
        seq_len = n;
    }

    return seq[n - 1U];
}
