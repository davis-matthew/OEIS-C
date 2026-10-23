/*
 * A018520  Divisors of 561.
 * https://oeis.org/A018520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 561 in increasing order
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Divisors of 561: 1,3,11,17,33,51,187,561 */
    static const seq_t divisors[8] = { 1, 3, 11, 17, 33, 51, 187, 561 };
    if (n == 0)
        return 0;               /* undefined for offset 0, return 0 */
    if (n <= 8)
        return divisors[n - 1];
    return 0;                   /* no divisor beyond the eighth one */
}
