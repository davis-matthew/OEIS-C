/*
 * A208131  Partial products of A052901.
 * https://oeis.org/A208131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^floor(n/3) * (r+1)*(r+2)/2, where r = n mod 3
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;              /* floor(n/3)           */
    seq_t pow = 1;
    for (idx_t i = 0; i < k; ++i)
        pow *= (seq_t)12;

    idx_t r = n % 3;
    seq_t factor = ((seq_t)(r + 1) * (seq_t)(r + 2)) / 2;

    return pow * factor;
}
