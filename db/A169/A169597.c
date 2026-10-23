/*
 * A169597  Numbers that are congruent to {2, 15} mod 19.
 * https://oeis.org/A169597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (38*n + 7*(-1)^n -23)/4
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified as: a(2k) = 19k-4 , a(2k+1) = 19k+2 */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; 0 is out of range – return 0 as a safe value */
        return 0;

    if ((n & 1) == 0) {                 /* n is even : n = 2k */
        seq_t k = (seq_t)(n >> 1);      /* k = n/2   */
        return (seq_t)19 * k - 4;
    } else {                            /* n is odd  : n = 2k+1 */
        seq_t k = (seq_t)((n - 1) >> 1); /* k = (n-1)/2 */
        return (seq_t)19 * k + 2;
    }
}
