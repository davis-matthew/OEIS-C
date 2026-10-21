/*
 * A063353  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          80 ).
 * https://oeis.org/A063353
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((-1)^(n+1) + 198*n - 423) / 2 for n > 2. a(2) = -1.
 *            Recurrence a(n) = a(n-1)+a(n-2)-a(n-3) for n > 5
 * Range:     i8 n <= 2, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined for n < 2; return 0 as placeholder */
    if (n == 2)
        return -1;              /* base case */

    /* Use the closed‑form formula for all n ≥ 3 */
    seq_t sn   = (seq_t)n;                 /* sequence index as seq_t */
    seq_t sign = ((n & 1ULL) ? (seq_t)1 : (seq_t)-1);   /* (-1)^(n+1) */
    seq_t num  = sign + (seq_t)198 * sn - (seq_t)423;
    return num / 2;
}
