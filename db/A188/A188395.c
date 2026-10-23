/*
 * A188395  a(n) = [n*r +k*r]-[n*r]-[k*r], where r=1/sqrt(2), k=4, [ ]=floor.
 * https://oeis.org/A188395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*r+4*r]-[n*r]-[4*r] , r = 1/sqrt(2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer scaling constants:
   SCALE = 2^40
   K     = floor(r * SCALE) = 777472126388
   Then floor(n * r)  = (n * K) >> 40
   floor((n+4) * r) = ((n+4) * K) >> 40
   Since floor(4*r) = 2, a(n) = floor((n+4)*r) - floor(n*r) - 2   */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^40 */
    static const uint64_t SCALE = ((uint64_t)1) << 40;
    /* floor((1/sqrt(2)) * SCALE) */
    static const uint64_t K = 777472126388ULL;   /* pre‑computed constant */

    /* use 128‑bit arithmetic to avoid overflow */
    __uint128_t prod_n  = (__uint128_t)n * K;
    __uint128_t prod_n4 = (__uint128_t)(n + 4ULL) * K;

    seq_t floor_n  = (seq_t)(prod_n >> 40);
    seq_t floor_n4 = (seq_t)(prod_n4 >> 40);

    return floor_n4 - floor_n - 2;
}
