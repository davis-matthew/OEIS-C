/*
 * A226546  Number of squares in all tilings of a 3 X n rectangle using
 *          integer-sided square tiles.
 * https://oeis.org/A226546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) -
 *            a(n-6)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 52, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=3, a(2)=12, a(3)=34, a(4)=98, a(5)=256            */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)34;
    if (n == 4) return (seq_t)98;
    if (n == 5) return (seq_t)256;

    /* allocate space for terms up to n */
    seq_t *mem = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    mem[0] = (seq_t)0;
    mem[1] = (seq_t)3;
    mem[2] = (seq_t)12;
    mem[3] = (seq_t)34;
    mem[4] = (seq_t)98;
    mem[5] = (seq_t)256;

    for (idx_t i = 6; i <= n; ++i) {
        mem[i] = (seq_t)(2) * mem[i - 1]
               + (seq_t)(3) * mem[i - 2]
               - (seq_t)(2) * mem[i - 3]
               - (seq_t)(6) * mem[i - 4]
               - (seq_t)(4) * mem[i - 5]
               - (seq_t)(1) * mem[i - 6];
    }

    seq_t result = mem[n];
    free(mem);
    return result;
}
