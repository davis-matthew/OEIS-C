/*
 * A186942  a(n)=2(4^n-n*2^n)-1.
 * https://oeis.org/A186942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (4^n - n*2^n) - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;   /* 2^n */
    seq_t pow4 = 1;   /* 4^n */

    for (idx_t i = 0; i < n; ++i) {
        pow2 = pow2 * 2;
        pow4 = pow4 * 4;
    }

    seq_t term = pow4 - pow2 * (seq_t)n;
    term = term * 2;
    term = term - 1;
    return term;
}
