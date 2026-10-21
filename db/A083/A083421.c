/*
 * A083421  a(n)=2*5^n-2^n.
 * https://oeis.org/A083421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*5^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 8, a(2) = 46, a(3) = 242, ... */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
        pow2 *= (seq_t)2;
    }
    return pow5 * (seq_t)2 - pow2;
}
