/*
 * A189462  n+[nr/t]+[ns/t]; r=2, s=sqrt(5), t=1+sqrt(5).
 * https://oeis.org/A189462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A189462
    Formula: a(n) = n + ⌊n/φ⌋ + ⌊n*(5-√5)/4⌋     (φ = (1+√5)/2)   */

/*  Compute integer square root using a binary method (no math.h).  */
static uint64_t
isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of 4 ≤ x */
    while (bit > x)
        bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t
a(idx_t n)
{
    /* Common typedefs provided by the build.  */
    uint64_t nn = (uint64_t)n;

    /* floor(n * sqrt(5))  */
    unsigned __int128 prod = (unsigned __int128)nn * nn * 5;
    uint64_t floor_n_sqrt5 = isqrt_u64((uint64_t)prod);

    /* ⌊n/φ⌋  = ⌊(floor(n√5) – n)/2⌋   */
    uint64_t div_phi = (floor_n_sqrt5 - nn) / 2;

    /* ⌊n*(5-√5)/4⌋  */
    unsigned __int128 n5_minus_sqrt5 = (unsigned __int128)5 * nn - floor_n_sqrt5;
    uint64_t div_beta = (uint64_t)(n5_minus_sqrt5 >> 2);   /* division by 4 */
    if ((n5_minus_sqrt5 & 3) == 0)                      /* adjust when (5n - floor(n√5)) % 4 == 0 */
        div_beta--;

    uint64_t full = nn + div_phi + div_beta;
    return (seq_t)full;
}
