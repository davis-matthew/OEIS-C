/*
 * A061656  Numbers k > 1 such that, in base 2, k and k^2 contain the same
 *          digits in the same proportion.
 * https://oeis.org/A061656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 3242, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9187 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate numbers k>1 whose binary representation and that of k²
   have the same proportion of 1‑bits.  Tested against all known terms
   of A061656. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_length.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Lazy memoised sequence */
    static seq_t *seq = NULL;          /* array of found k's */
    static size_t  cap = 0, len = 0;   /* current capacity and length */
    static seq_t  k = 3;                /* next candidate to test */

    /* Grow the sequence until we have at least the requested index. */
    while (len < (size_t)n)
    {
        /* bit length of current k */
        idx_t   len_k  = bit_length(k);
        /* count of 1‑bits in k */
        unsigned int ones_k = 0;
        seq_t tmp = k;
        while (tmp)
        {
            ones_k += (unsigned int)(tmp & 1);
            tmp >>= 1;
        }

        /* square */
        seq_t k2 = k * k;
        /* bit length of k² */
        idx_t len_k2 = bit_length(k2);
        /* count of 1‑bits in k² */
        unsigned int ones_k2 = 0;
        tmp = k2;
        while (tmp)
        {
            ones_k2 += (unsigned int)(tmp & 1);
            tmp >>= 1;
        }

        /* check proportion: ones_k/len_k == ones_k2/len_k2  ⇔
           ones_k * len_k2 == ones_k2 * len_k */
        if ((seq_t)ones_k * (seq_t)len_k2 == (seq_t)ones_k2 * (seq_t)len_k)
        {
            /* store found value */
            if (len == cap)
            {
                size_t new_cap = (cap == 0) ? 32 : cap << 1;
                seq = seq_realloc(seq, cap, new_cap);
                cap = new_cap;
            }
            seq[len++] = k;
        }
        k++; /* next candidate */
    }
    return seq[n - 1];
}
