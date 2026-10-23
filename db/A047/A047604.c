/*
 * A047604  Numbers that are congruent to {2, 3, 5} mod 8.
 * https://oeis.org/A047604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k-2) = 8k-6, a(3k-1) = 8k-5, a(3k) = 8k-3, k ≥ 1
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is the increasing list of numbers that are 2, 3 or 5 (mod 8). */
    if (n == 0) return 0;                 /* not used, safety */

    int r = (int)(n % 3);                 /* remainder modulo 3 */
    seq_t k, val;                         /* work in the result type */

    switch (r)
    {
        case 1:                            /* n = 3k-2  => k = (n+2)/3 */
            k = ((seq_t)n + 2) / 3;
            val = 8 * k - 6;
            break;

        case 2:                            /* n = 3k-1  => k = (n+1)/3 */
            k = ((seq_t)n + 1) / 3;
            val = 8 * k - 5;
            break;

        default:                            /* r == 0, n = 3k       */
            k = (seq_t)n / 3;
            val = 8 * k - 3;
            break;
    }
    return val;
}
