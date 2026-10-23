/*
 * A017500  a(n) = (11*n + 9)^4.
 * https://oeis.org/A017500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 9)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 22, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base value in the widest type available.
       Casting both operands to seq_t ensures the multiplication
       occurs in seq_t arithmetic (or in unsigned __int128
       underneath if seq_t is 128‑bit). */
    seq_t base = (seq_t)11 * (seq_t)n + (seq_t)9;

    /* Use an intermediate 128‑bit type to prevent overflow
       during the squaring steps.  The result will be accurate
       for all n that fit in seq_t; for larger n the truncation
       to the lower bits is acceptable because those values
       are not required to be correct by the problem statement. */
    unsigned __int128 tmp = (unsigned __int128)base;
    tmp = tmp * tmp;      /* base² */
    tmp = tmp * tmp;      /* base⁴ */

    return (seq_t)tmp;
}
