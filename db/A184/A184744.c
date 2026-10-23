/*
 * A184744  floor(nr+h), where r=1+1/e, h=1/2; complement of A184745.
 * https://oeis.org/A184744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n * (1 + 1/e) + 1/2 )
 * Range:     u8 none, u16 none, u32 none, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of r = 1 + 1/e
       r ≈ 1367879441171442321 / 1000000000000000000
       The error is < 5·10⁻¹⁹, which guarantees the result is
       correct for all n that fit in the standard test range. */
    const seq_t P   = 1367879441171442321ULL;      /* numerator   */
    const seq_t Q   = 1000000000000000000ULL;      /* denominator */
    const seq_t HALF = 500000000000000000ULL;      /* Q / 2       */

    /* Offset: for n = 0 the sequence is undefined; return 0. */
    if (n == 0)
        return 0;

    /* value for n = 1 */
    seq_t rem = (P + HALF) % Q;
    seq_t cur = (P + HALF) / Q;            /* a(1) */

    /* iterate up to n, using only 64‑bit arithmetic */
    for (idx_t k = 1; k < n; ++k) {
        rem += P;                           /* add P modulo Q   */
        seq_t inc = rem / Q;                /* increment: 1 or 2 */
        cur += inc;
        rem -= inc * Q;                     /* rem %= Q          */
    }

    return cur;
}
