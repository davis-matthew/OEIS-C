/*
 * A171597  a(n) = 0+1+2+...+n in lunar arithmetic in base 7 written in base
 *          10.
 * https://oeis.org/A171597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (msd_of_n_in_base7 + 1) * 7^k - 1, where k =
 *            floor(log_7(n)) and msd_of_n_in_base7 = n / 7^k. In base‑7 this
 *            means keep the most significant digit of n, replace all lower
 *            digits with 6, and interpret the result in decimal
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;                /* 7^k, initially 1 (7^0)              */
    seq_t sn  = (seq_t)n;         /* cast once to avoid repeated casts     */

    /* Find the greatest power of 7 that does not exceed n. */
    while (pow <= sn / 7)
        pow *= 7;

    seq_t d = sn / pow;           /* most significant digit of n in base 7 */
    return (d + 1) * pow - 1;     /* construct the lunar sum in decimal   */
}
