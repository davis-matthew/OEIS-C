/*
 * A000909  a(n) = (2*n)!*(2*n+1)! / n!^2.
 * https://oeis.org/A000909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)! * (2*n+1)! / n!^2 = ((2n)!/n!)^2 * (2*n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t prod = 1;
    seq_t n_seq = (seq_t)n;
    seq_t two_n = 2 * n_seq;
    
    /* Compute (2*n)! / n! = (n+1) * (n+2) * ... * (2*n) */
    for (seq_t i = n_seq + 1; i <= two_n; i++) {
        prod *= i;
    }
    
    /* a(n) = ((2n)!/n!)^2 * (2*n+1) */
    seq_t result = prod * prod * (two_n + 1);
    
    return result;
}
