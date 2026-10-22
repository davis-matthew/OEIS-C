/*
 * A005072  Sum of cubes of primes = 1 mod 3 dividing n.
 * https://oeis.org/A005072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p^3 if p ≡ 1 (mod 3), 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1626, u64 n <= 9204,
 *            u128 n <= 9204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;
    
    // Trial division by 2
    if (m % 2 == 0) {
        while (m % 2 == 0) m /= 2;
    }
    
    // Trial division by odd numbers starting from 3
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (p % 3 == 1) {
                result += (seq_t)p * p * p;
            }
            while (m % p == 0) m /= p;
        }
    }
    
    // Remaining factor m > 1 must be prime
    if (m > 1 && m % 3 == 1) {
        result += (seq_t)m * m * m;
    }
    
    return result;
}
