/*
 * A040365  Continued fraction for sqrt(385).
 * https://oeis.org/A040365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) gives the nth partial quotient in the continued * fraction
 *            of sqrt(385). The sequence is periodic after * the initial term
 *            a(0)=19 with period length 16: * * period =
 *            [1,1,1,1,1,3,1,2,1,3,1,1,1,1,1,38] * * (Indices are 0‑based.)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(385) */
    if (n == 0)
        return (seq_t)19;

    /* The periodic part of the continued fraction. */
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)3, (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)3,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)38
    };

    /* the index into the period is (n-1) mod 16 */
    idx_t idx = (n - 1) % 16;
    return period[idx];
}
