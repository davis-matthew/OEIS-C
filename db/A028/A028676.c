/*
 * A028676  Pseudo Galois numbers for d=14.
 * https://oeis.org/A028676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14^n * Product_{k=0..n-1} (14^n - 14^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power_n = 1;
    for (idx_t i = 0; i < n; ++i) {
        power_n *= 14;
    }

    seq_t result = power_n;
    seq_t power_k = 1;
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = power_n - power_k;
        result *= term;
        power_k *= 14;
    }

    return result;
}
