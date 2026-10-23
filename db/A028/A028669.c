/*
 * A028669  Galois numbers for p=7; order of group AGL(n,7).
 * https://oeis.org/A028669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n * ∏_{k=0..n-1} (7^n - 7^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 7^n */
    seq_t pow7n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow7n *= (seq_t)7;

    seq_t result = pow7n;
    seq_t pow7k = 1;
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = pow7n - pow7k;
        result *= term;
        pow7k *= (seq_t)7;
    }
    return result;
}
