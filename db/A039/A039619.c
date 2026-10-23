/*
 * A039619  Second column of Jabotinsky-triangle A038455 related to A006963.
 * https://oeis.org/A039619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n-1)! / n!) * (H(2*n-1)-H(n))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 2 for the sequence */
    seq_t product = 1;
    idx_t limit = 2 * n - 1;
    for (idx_t i = n + 1; i <= limit; ++i)
        product *= (seq_t)i;

    seq_t sum = 0;
    for (idx_t i = n + 1; i <= limit; ++i)
        sum += product / (seq_t)i;

    return sum;
}
