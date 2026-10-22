/*
 * A007538  A self-generating sequence: there are a(n) 3's between successive
 *          2's.
 * https://oeis.org/A007538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor( n*(1+sqrt(3)) ) - floor( (n-1)*(1+sqrt(3)) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const uint64_t SCALE = 1000000000000000000ULL;
    const uint64_t alpha_scaled = 2732050807568877293ULL;
    
    unsigned __int128 temp_n = (unsigned __int128) n * alpha_scaled;
    uint64_t floor_n = (uint64_t)(temp_n / SCALE);
    
    unsigned __int128 temp_n_m1 = (unsigned __int128)(n - 1) * alpha_scaled;
    uint64_t floor_n_m1 = (uint64_t)(temp_n_m1 / SCALE);
    
    return (seq_t)(floor_n - floor_n_m1);
}
