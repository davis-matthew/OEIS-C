/*
 * A126679  Product_{i=3..n} Stirling_2(i,3).
 * https://oeis.org/A126679
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=3..n} Stirling_2(i,3)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* undefined below the offset */

    seq_t product = (seq_t)1;          /* product of the terms */
    seq_t p3 = (seq_t)27;              /* 3^3 */
    seq_t p2 = (seq_t)8;               /* 2^3 */

    for (idx_t i = 3; ; ++i)
    {
        /* S(i,3) = (3^i - 3·2^i + 3) / 6 */
        seq_t term = (p3 - (seq_t)3 * p2 + (seq_t)3) / (seq_t)6;
        product *= term;

        if (i == n)
            break;
        p3 *= (seq_t)3;               /* advance 3^i to 3^(i+1) */
        p2 *= (seq_t)2;               /* advance 2^i to 2^(i+1) */
    }

    return product;
}
