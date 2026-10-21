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
 * Helpers:   bitlen_seq_t, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate numbers k>1 whose binary representation and that of k²
   have the same proportion of 1‑bits.  Tested against all known terms
   of A061656. */
#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/bitlen_seq_t.h"

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
        idx_t   len_k  = bitlen_seq_t(k);
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
        idx_t len_k2 = bitlen_seq_t(k2);
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
