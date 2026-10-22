/*
 * A003960  Fully multiplicative with a(p) = [ (p+1)/2 ] for prime p.
 * https://oeis.org/A003960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product [(p(k)+1)/2]^e(k) for n = Product p(k)^e(k)
 * Range:     u8 n <= 520, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    
    seq_t result = 1;
    idx_t m = n;
    
    // Factor out 2's
    if (m % 2 == 0) {
        while (m % 2 == 0)
            m /= 2;
        // (2+1)/2 = 1, contributes 1^e = 1 to product
    }
    
    // Check odd prime factors
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            seq_t factor = (seq_t)((p + 1) / 2);
            seq_t power = 1;
            for (idx_t i = 0; i < exp; i++)
                power *= factor;
            result *= power;
        }
    }
    
    // Handle remaining prime factor
    if (m > 1) {
        seq_t factor = (seq_t)((m + 1) / 2);
        result *= factor;
    }
    
    return result;
}
