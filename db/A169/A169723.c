/*
 * A169723  a(n) = 3^(n-1)*(2*3^(n-1)+3)+1.
 * https://oeis.org/A169723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*9^(n-1) + 3^n + 1 (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n < 1 we return 0, although the sequence is defined for n >= 1. */
    if (n < 1) return (seq_t)0;

    seq_t pow3 = (seq_t)1;          /* 3^n */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        pow3 *= (seq_t)3;
    }

    seq_t pow9 = (seq_t)1;          /* 9^(n-1) */
    for (idx_t i = (idx_t)1; i < n; ++i) {
        pow9 *= (seq_t)9;
    }

    return (seq_t)2 * pow9 + pow3 + (seq_t)1;
}
