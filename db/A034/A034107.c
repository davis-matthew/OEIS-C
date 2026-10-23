/*
 * A034107  Fractional part of square root of a(n) starts with 1: first term
 *          of runs.
 * https://oeis.org/A034107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the first integer of each maximal run of consecutive *
 *            integers k for which the fractional part of sqrt(k) lies in the
 *            interval * [0.1, 0.2). That is, 0.1 ≤ frac(sqrt(k)) < 0.2. The
 *            value a(1) = 10
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m      = 1;          /* current value of floor(sqrt(k))   */
    idx_t runCnt = 0;          /* number of runs found so far     */

    for (;; ++m) {
        /* compute the bounds of the run corresponding to this m:  */
        /* lower = ceil((m + 0.1)^2)   = m^2 + (20*m + 100)/100  */
        /* upper = floor((m + 0.2)^2) = m^2 + (40*m + 4)/100   */
        idx_t m2   = m * m;
        idx_t lower = m2 + (20 * m + 100) / 100;
        idx_t upper = m2 + (40 * m + 4)   / 100;

        if (lower <= upper) {      /* this m produces a non‑empty run   */
            ++runCnt;
            if (runCnt == n) {
                return (seq_t)lower;   /* first term of the n‑th run    */
            }
        }
    }
}
