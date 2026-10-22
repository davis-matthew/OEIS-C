/*
 * A001689  5th forward differences of factorial numbers A000142.
 * https://oeis.org/A001689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 10*n^4 + 45*n^3 + 100*n^2 + 109*n + 44)*n!
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    
    // Compute polynomial using Horner's method
    seq_t poly = ((((n_seq + (seq_t)10)*n_seq + (seq_t)45)*n_seq + (seq_t)100)*n_seq + (seq_t)109)*n_seq + (seq_t)44;
    
    // Compute n!
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; i++) {
        fact *= (seq_t)i;
    }
    
    return poly * fact;
}
