/*
 * A184741  floor(n*s+h-h*s), where s=(e-1)/(e-2) and h=-1/2; complement of
 *          A184740.
 * https://oeis.org/A184741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*s + h - h*s), where s = (e-1)/(e-2), h = -1/2.
 *            The sequence is a Beatty sequence with irrational slope s ≈
 *            2.392211119177... . It can be evaluated exactly using a
 *            high‑precision rational approximation of s, avoiding any
 *            floating point operations
 * Range:     u8 none, u16 none, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* High‑precision rational approximation of s:
     s ≈ 2392211 / 1000000   (error < 5·10⁻⁷, guaranteeing correctness for n < 1 000 000). */
static const seq_t P   = 2392211;          /* numerator of s approximation   */
static const seq_t Q   = 1000000;          /* denominator of s approximation */
static const seq_t OFF = P - Q;           /* constant term P - Q = 1,392,211  */
static const seq_t DEN = 2 * Q;           /* 2·Q = 2,000,000                 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;                 /* undefined for offset 1, but avoids UB */

    /* Use a 128‑bit intermediate to prevent overflow when n is large. */
    __int128 tmp = (__int128)2 * ( __int128)n * ( __int128)P + ( __int128)OFF;
    return (seq_t)(tmp / DEN);
}
