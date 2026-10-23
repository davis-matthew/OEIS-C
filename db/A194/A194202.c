/*
 * A194202  a(n) = [Sum_{k=1..n} (2k*e)], where [ ]=floor, ( )=fractional
 *          part.
 * https://oeis.org/A194202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(∑_{k=1..n} { 2k e } ), where {x} = x - floor(x)
 * Range:     u8 n <= 511, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a 15‑digit rational approximation of 2e:
 *   2e ≈ 5.43656365691809  = 5436563656918090 / 10^15
 * The error of this approximation is <10⁻¹⁵, which is far below the
 * threshold needed to keep floor(k·2e) correct for k ≤ 10 000.
 * Fractional parts are accumulated in scaled form and the final floor
 * is obtained by dividing by 10^15. */
 
#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* 2e ≈ 5436563656918090 / 10^15 */
    const uint64_t NUM   = 5436563656918090ULL;
    const uint64_t DEN   = 1000000000000000ULL;  /* 10^15 */
 
    unsigned __int128 sum_scaled = 0;           /* accumulator for k*NUM (mod DEN) */
 
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute (k * NUM) % DEN using 128‑bit intermediate to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)k * NUM;
        uint64_t term = (uint64_t)(prod % DEN); /* fractional part * DEN */
        sum_scaled += term;
    }
 
    /* The desired result is floor( sum_scaled / DEN ) */
    unsigned __int128 quotient = sum_scaled / DEN;
    return (seq_t)quotient;
}
