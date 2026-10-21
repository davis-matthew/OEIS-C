/*
 * A082968  a(n)=sum(k=1,n, k mod sum(i=0,k-1,1-t(i))) where t(i)=A010060(i)
 *          is the Thue-Morse sequence.
 * https://oeis.org/A082968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=sum_{k=1}^{n} k mod (count of i<k with popcount(i) even)
 * Range:     u8 n <= 60, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for the OEIS sequence A082968.
   n is a 1‑based index.  The function works for any integer type seq_t
   (unsigned 8–128 bit or arbitrary precision) provided the usual
   arithmetic operators are defined. */
seq_t a(idx_t n)
{
    idx_t even_cnt = 0;        /* number of i with even popcount in [0, k-1] */
    seq_t sum = 0;             /* running total of a(n) */

    for (idx_t i = 0; i < n; ++i)
    {
        /* i corresponds to the element k-1 in the inner sum */
        if (((__builtin_popcountll((unsigned long long)i) & 1) == 0))
            ++even_cnt;

        seq_t k = (seq_t)(i + 1);
        seq_t s = (seq_t)even_cnt;   /* s(k) */
        sum += k % s;
    }
    return sum;
}
