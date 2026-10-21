/*
 * A084215  Expansion of g.f.: (1+x^2)/(1-2*x).
 * https://oeis.org/A084215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^n + (n>=2 ? 2^(n-2) : 0)
 * Expansion of g.f.: (1+x^2)/(1-2*x), offset 0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = 1;      /* current 2^i */
    seq_t res  = 0;      /* accumulator for a(n) */
    idx_t i;

    /* Iterate i from 0 to n, accumulating the two terms that match n. */
    for (i = 0; i <= n; ++i) {
        if (i == n)                 /* term 2^n */
            res += pow2;
        if (i + 2 == n)             /* term 2^(n-2) when n>=2 */
            res += pow2;

        if (i == n)                 /* finished; no need to compute next power */
            break;

        pow2 = pow2 * 2;            /* next power of two */
    }

    return res;
}
