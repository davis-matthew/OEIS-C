/*
 * A127316  a(n) = 2*n^2 - 4*n + 73.
 * https://oeis.org/A127316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2 - 4*n + 73
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t first to keep all arithmetic in that type */
    seq_t sn = (seq_t)n;

    /* Handle small n to avoid underflow in (sn - 2) */
    if (sn < 2)
        return (seq_t)71;          /* a(1) = 71, a(0) not in the sequence */

    /* For n >= 2: a(n) = 2 * n * (n - 2) + 73 */
    seq_t val = sn * (sn - (seq_t)2); /* n * (n - 2)  */
    val = val * (seq_t)2;             /* 2 * n * (n - 2) */
    val = val + (seq_t)73;            /* + 73 */
    return val;
}
