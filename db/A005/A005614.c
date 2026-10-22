/*
 * A005614  The binary complement of the infinite Fibonacci word A003849.
 *          Start with 1, apply 0->1, 1->10, iterate, take limit.
 * https://oeis.org/A005614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+2)*u) - floor((n+1)*u), where u = (sqrt(5) -
 *            1)/2
 * Range:     u8 n <= 7887, u16 n <= 7887, u32 n <= 7887, u64 n <= 7887,
 *            u128 n <= 7887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Rational approximation for u using continued fraction convergent:
    // sqrt(5) ≈ 2889/1292, so u ≈ (2889 - 1292)/(2*1292) = 1597/2584
    
#ifdef __SIZEOF_INT128__
    unsigned __int128 n1 = (unsigned __int128)n + 1;
    unsigned __int128 n2 = (unsigned __int128)n + 2;
    unsigned __int128 f1 = (n1 * 1597ULL) / 2584ULL;
    unsigned __int128 f2 = (n2 * 1597ULL) / 2584ULL;
#else
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;
    idx_t f1 = (n1 * 1597ULL) / 2584ULL;
    idx_t f2 = (n2 * 1597ULL) / 2584ULL;
#endif
    
    return (seq_t)(f2 - f1);
}
