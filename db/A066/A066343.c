/*
 * A066343  Beatty sequence for log_2(10).
 * https://oeis.org/A066343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*log_2(10))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  log_2(10) = 3.321928094887362347870319429...  */
/*  We use a fraction p/q = 3321928094887362 / 10^15, 15 decimals accurate. */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1; undefined for n=0, return 0 for safety */
        return (seq_t)0;
    }

    /* High‑precision rational approximation of log_2(10) */
    const seq_t P = 3321928094887362ULL;          /* numerator: 3.321928094887362 */
    const seq_t Q = 1000000000000000ULL;          /* denominator: 10^15 */

    /* Compute floor(n * P / Q) using integer arithmetic only */
    seq_t prod = (seq_t)n * P;
    return prod / Q;
}
