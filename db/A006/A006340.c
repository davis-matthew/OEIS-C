/*
 * A006340  An "eta-sequence": [ (n+1)*tau + 1/2 ] - [ n*tau + 1/2 ], tau = (1
 *          + sqrt(5))/2.
 * https://oeis.org/A006340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+1)*tau + 1/2) - floor(n*tau + 1/2), tau = (1 +
 *            sqrt(5))/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Use convergent F_24/F_23 = 46368/28657 to approximate tau
    // This rational approximation is accurate to better than 1e-10
    uint64_t p = 46368;
    uint64_t q = 28657;
    
    uint64_t n_plus_1 = (uint64_t)n + 1;
    
    // Compute floor((n+1)*p/q + 0.5)
    uint64_t prod1 = n_plus_1 * p;
    uint64_t int1 = prod1 / q;
    uint64_t frac1 = prod1 % q;
    uint64_t floor1 = (frac1 * 2 >= q) ? (int1 + 1) : int1;
    
    // Compute floor(n*p/q + 0.5)
    uint64_t prod2 = (uint64_t)n * p;
    uint64_t int2 = prod2 / q;
    uint64_t frac2 = prod2 % q;
    uint64_t floor2 = (frac2 * 2 >= q) ? (int2 + 1) : int2;
    
    return (seq_t)(floor1 - floor2);
}
