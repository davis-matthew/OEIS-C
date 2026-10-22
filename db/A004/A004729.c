/*
 * A004729  Divisors of 2^32 - 1 (for a(1) to a(31), the 31 known regular
 *          polygons with an odd number of sides constructible with ruler and
 *          compass).
 * https://oeis.org/A004729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy, changed
 * Formula:   a(n) = product of Fermat primes {3, 5, 17, 257, 65537}
 *            corresponding to bits set in n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t fermat_primes[] = {3, 5, 17, 257, 65537};
    seq_t result = 1;
    
    for (int i = 0; i < 5; i++) {
        if (n & (1ULL << i)) {
            result *= fermat_primes[i];
        }
    }
    
    return result;
}
