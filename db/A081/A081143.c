/*
 * A081143  5th binomial transform of (0,0,0,1,0,0,0,0,......).
 * https://oeis.org/A081143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n,3) * 5^(n-3), with binomial(n,3)=0 for n<3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=0, a(3)=1; a(n)=20*a(n-1)-150*a(n-2)+500*a(n-3)-625*a(n-4). */
#include <stddef.h>
#include <stdint.h>

/* Computes the nth term of sequence A081143. */
seq_t a(idx_t n)
{
    /* Terms a(0), a(1), a(2) are zero. */
    if (n < 3)
        return 0;

    /* binomial(n,3) = n*(n-1)*(n-2)/6 */
    seq_t bin = (seq_t)n * ((seq_t)n - 1) * ((seq_t)n - 2);
    bin /= 6;

    /* 5^(n-3) */
    seq_t pow5 = 1;
    idx_t exp = n - 3;
    for (idx_t i = 0; i < exp; ++i)
        pow5 *= 5;

    return bin * pow5;
}
