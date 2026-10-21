/*
 * A216407  Sum of decimal digits not appearing in n.
 * https://oeis.org/A216407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 45 - sum_{d in digits(n)} d
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned char seen[10] = {0};
    idx_t m = n;
    if (m == 0)
        seen[0] = 1;
    while (m > 0) {
        unsigned d = m % 10;
        seen[d] = 1;
        m /= 10;
    }
    seq_t sum_present = 0;
    for (int d = 0; d < 10; ++d)
        if (seen[d])
            sum_present += (seq_t)d;
    return (seq_t)45 - sum_present;
}
