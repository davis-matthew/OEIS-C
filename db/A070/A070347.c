/*
 * A070347  a(n) = 2^n mod 21.
 * https://oeis.org/A070347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 21
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^n mod 21 has period 6 */
    static const seq_t tbl[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                  (seq_t)8, (seq_t)16, (seq_t)11 };
    return tbl[n % 6];
}
