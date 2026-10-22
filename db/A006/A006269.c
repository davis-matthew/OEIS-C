/*
 * A006269  A continued cotangent.
 * https://oeis.org/A006269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^3 + 3*a(n), a(0) = 5
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 5;
    
    for (idx_t i = 0; i < n; i++) {
        result = result * result * result + 3 * result;
    }
    
    return result;
}
