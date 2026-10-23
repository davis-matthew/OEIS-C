/*
 * A179382  a(n) is the smallest period of pseudo-arithmetic progression with
 *          initial term 1 and difference 2n-1.
 * https://oeis.org/A179382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the length of the cycle of the mapping * f(x) =
 *            odd_part(x + (2*n-1)) (starting from x = 1), * where odd_part(y)
 *            removes all factors of 2 from y
 * Range:     u8 n <= 64, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 2*n-1 fits in seq_t since n is at most a few ten‑thousand */
    seq_t M = (seq_t)n * ((seq_t)2) - ((seq_t)1);

    seq_t cur = ((seq_t)1);
    seq_t len = ((seq_t)0);

    do
    {
        seq_t temp = cur + M;
        /* remove all powers of 2 from temp */
        while ((temp & ((seq_t)1)) == ((seq_t)0))
            temp >>= 1;
        cur = temp;
        ++len;
    } while (cur != ((seq_t)1));

    return len;
}
