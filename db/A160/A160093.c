/*
 * A160093  Number of digits in n, excluding any trailing zeros.
 * https://oeis.org/A160093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of digits in n after removing all trailing zeros
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Strip trailing zeros from n. */
    idx_t m = n;
    while ((m % (idx_t)10u) == 0u)
        m /= (idx_t)10u;

    /* Count the remaining digits. */
    seq_t len = 0u;
    do {
        len++;
        m /= (idx_t)10u;
    } while (m > 0u);

    return len;
}
