/*
 * A084953  Numbers k such that k! is the sum of 4 but no fewer nonzero
 *          squares.
 * https://oeis.org/A084953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that k! can be written as a sum
 *            of exactly 4 non‑zero squares. Equivalently, k! = 4^i · (8j+7)
 *            for some i,j≥0. This property holds iff the exponent of 2 in k!
 *            is even and the odd part of k! is congruent to 7 mod 8
 * Range:     u8 n <= 32, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state to memoise the sequence */
    static seq_t *seq = NULL;
    static idx_t seq_cap   = 0;
    static idx_t seq_len   = 0;
    static idx_t next_k    = 1;        /* Next integer k to test        */
    static idx_t total_twos = 0;       /* v2(k!) accumulated so far    */
    static uint8_t odd_mod8  = 1;      /* (k! / 2^{v2}) mod 8 so far   */

    /* Extend the cached sequence until we have at least n terms */
    while (seq_len < n)
    {
        /* Factorise current integer next_k */
        idx_t k = next_k;
        idx_t tmp = k;
        idx_t twos_k = 0;

        /* Count trailing zeros of k (i.e., power of 2 dividing k) */
        while ((tmp & 1ULL) == 0)
        {
            tmp >>= 1;
            ++twos_k;
        }

        /* Update total number of 2’s in k! */
        total_twos += twos_k;

        /* Update odd part modulo 8 */
        odd_mod8 = ( (uint8_t)odd_mod8 * (uint8_t)(tmp & 7) ) & 7;

        /* Check whether k! satisfies 4^i·(8j+7) conditions */
        if ( (total_twos & 1ULL) == 0 && odd_mod8 == 7 )
        {
            /* Append k to the cache */
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)k;
        }

        /* Move to the next integer */
        ++next_k;
    }

    /* Return the (n‑1)th zero‑based element of the cache */
    return seq[n - 1];
}
