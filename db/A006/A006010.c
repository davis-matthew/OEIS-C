/*
 * A006010  Number of paraffins (see Losanitsch reference for precise
 *          definition).
 * https://oeis.org/A006010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n is odd, a(n) = (1/8) * (n^4 + 2*n^3 + 2*n^2 + 2*n + 1); if
 *            n is even, a(n) = (1/8) * (n^4 + 2*n^3 + 2*n^2)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t n2 = n * n;
    idx_t n3 = n2 * n;
    idx_t n4 = n3 * n;
    
    idx_t result;
    if (n & 1) {
        result = (n4 + 2*n3 + 2*n2 + 2*n + 1) / 8;
    } else {
        result = (n4 + 2*n3 + 2*n2) / 8;
    }
    
    return (seq_t)result;
}
