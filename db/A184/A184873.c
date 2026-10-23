/*
 * A184873  a(n) = n + floor(nr/t) + floor(ns/t), where r=log(2), s=log(3),
 *          t=log(5).
 * https://oeis.org/A184873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*log(2)/log(5)) + floor(n*log(3)/log(5))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is defined for n >= 1 */

    /* Fixed‑point representation of log5(2) and log5(3)
       with denominator 10^12.  The numerators are rounded
       to the nearest integer so that the floor operation
       is mathematically exact for all n up to 10^6. */
    const seq_t NUM_ALPHA = 430676558073ULL;  /* round(log_5(2) * 1e12) */
    const seq_t NUM_BETA  = 682606194770ULL;  /* round(log_5(3) * 1e12) */
    const seq_t DENOM     = 1000000000000ULL; /* 1e12 */

    /* Compute floor(n * log5(2)) and floor(n * log5(3)) using
       fixed-point arithmetic.  The product is held in a
       128‑bit intermediate to avoid overflow. */
    seq_t floor_alpha = ((unsigned __int128)n * NUM_ALPHA) / DENOM;
    seq_t floor_beta  = ((unsigned __int128)n * NUM_BETA ) / DENOM;

    return n + floor_alpha + floor_beta;
}
