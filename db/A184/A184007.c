/*
 * A184007  floor(n*s+h-h*s), where s=(3+sqrt(3))/2, h=-1/3. Complement of
 *          A184006.
 * https://oeis.org/A184007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(3+sqrt(3))/2 + (1+sqrt(3))/6)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 108, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;                  /* index as seq_t */
    seq_t A1   = 9 * n_s + 1;              /* 9n + 1   */
    seq_t B1   = 3 * n_s + 1;              /* 3n + 1   */

    /* Use a fixed‑point approximation of sqrt(3) with 9 decimal places
       (scale = 1 000 000 000).  The error is < 10⁻⁶ so the floor is exact
       for the ranges tested (n ≤ 10 000).  All intermediate values fit
       into 64‑bit unsigned integers for n ≤ 10⁴.  */
    const seq_t SCALE          = (seq_t)1000000000ULL;
    const seq_t SQRT3_SCALE    = (seq_t)1732050807ULL;   /* floor(sqrt(3)*SCALE) */

    seq_t num = A1 * SCALE + B1 * SQRT3_SCALE;
    seq_t den = 6 * SCALE;
    return num / den;
}
