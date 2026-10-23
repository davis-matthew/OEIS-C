/*
 * A104615  Write the natural numbers in base 12 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = base‑12 digit at position n(n+1)/2 in the concatenated
 *            digit * sequence of natural numbers written in base‑12 (most
 *            significant digit first)
 * Range:     u8 n <= 23, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, may be used by the build */

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for n=0, return 0 safely */
        return 0;

    /* compute the triangular index in seq_t to avoid intermediate overflow */
    seq_t target = (seq_t)n * ((seq_t)n + 1) / 2;   /* n(n+1)/2 */

    /* walk through the numbers, counting digits, until we reach the target */
    idx_t  num = 1;              /* current natural number */
    seq_t  cum = 0;              /* cumulative digit count so far */

    while (1) {
        /* count digits of 'num' in base 12 */
        idx_t tmp = num;
        int    dlen = 0;
        while (tmp > 0) {          /* at least one digit */
            tmp /= 12;
            ++dlen;
        }

        /* if the target lies within the digits of this number, stop */
        if (cum + (seq_t)dlen >= target) {
            /* find offset [0..dlen-1] of the wanted digit (MSB first) */
            idx_t offset = (idx_t)(target - cum - 1);   /* 0‑based from MSB */

            /* build the reversed digit sequence of 'num' */
            seq_t rev[64];      /* enough for any 64‑bit number in base 12 */
            int    revlen = 0;
            tmp = num;
            while (tmp > 0) {
                rev[revlen++] = (seq_t)(tmp % 12);
                tmp /= 12;
            }
            /* if num were 0 (never happens here), would set rev[0]=0 */

            /* retrieve the digit: most significant is rev[revlen-1] */
            seq_t digit = rev[revlen - 1 - (int)offset];
            return digit;
        }

        /* otherwise, consume this number's digits and move to the next one */
        cum += (seq_t)dlen;
        ++num;
    }
}
