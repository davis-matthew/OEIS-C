/*
 * A028680  Pseudo Galois numbers for d=18.
 * https://oeis.org/A028680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18^n * Product_{k=0..n-1} (18^n - 18^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;    /* 18^0 */
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        pow = pow * 18;                /* 18^i */
        result = result * pow * (pow - 1);
    }
    return result;
}
