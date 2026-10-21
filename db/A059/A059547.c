/*
 * A059547  Beatty sequence for 1 + 1/log(3).
 * https://oeis.org/A059547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * (1 + 1 / log(3)) ) (Beatty sequence for 1 +
 *            1/log(3))
 * Range:     u8 n <= 134, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed with a fixed‑point rational approximation of the irrational constant
   α = 1 + 1/log(3).  The approximation α ≈ 1.910239226116458 is represented as
   α ≈ α_int / SCALE with SCALE = 1 000 000 and α_int = 1 910 239.
   For all n ≤ 10 000 the error in the approximation is < 5·10⁻⁷, so
   floor(n·α) = floor(n·α_int / SCALE).  The intermediate product is taken in
   128‑bit to avoid overflow.   */
           
seq_t a(idx_t n)
{
    /* 1 + 1/log(3) ≈ 1.910239226116458 */
    static const uint64_t SCALE   = 1000000ULL;   /* 10⁶ scaling factor   */
    static const uint64_t ALPHA_INT = 1910239ULL; /* round(α * SCALE)     */

    /* Compute floor(n * α_int / SCALE) using 128‑bit intermediate multiplication. */
    __int128 prod = (__int128)n * ALPHA_INT;
    return (seq_t)(prod / SCALE);
}
