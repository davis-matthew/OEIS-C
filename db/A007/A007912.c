/*
 * A007912  Quantum factorials: (n-1)!! - (n-2)!! (mod n).
 * https://oeis.org/A007912
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((n-1)!! - (n-2)!!) mod n
 * Range:     u8 n <= 261, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    
    // Compute (n-1)!! mod n
    seq_t dd1 = 1;
    for (idx_t i = n - 1; i >= 1; i -= 2) {
        dd1 = (dd1 * (seq_t)(i % n)) % n_seq;
        if (i <= 2) break;
    }
    
    // Compute (n-2)!! mod n
    seq_t dd2 = 1;
    for (idx_t i = n - 2; i >= 1; i -= 2) {
        dd2 = (dd2 * (seq_t)(i % n)) % n_seq;
        if (i <= 2) break;
    }
    
    // Return (dd1 - dd2) mod n, handled correctly
    return (dd1 - dd2 + n_seq) % n_seq;
}
