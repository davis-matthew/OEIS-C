/*
 * A042838  Numerators of continued fraction convergents to sqrt(950).
 * https://oeis.org/A042838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of the convergents to √950:
   a(0) = 30, a(1) = 31,
   for n ≥ 1: a(n) = q[(n‑1) mod 16] * a(n‑1) + a(n‑2)
   where q = [1, 4, 1, 1, 1, 1, 1, 2,
              1, 1, 1, 1, 1, 4, 1, 60] */

/* The function is order‑independent and runs in O(n) time.  It uses
   only integer arithmetic and no external libraries beyond the
   types seq_t and idx_t supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Handle the first term directly. */
    if (n == 0) return (seq_t)30;

    /* Partial quotients of the periodic continued fraction for √950. */
    static const int q[16] = {
        1, 4, 1, 1, 1, 1, 1, 2,
        1, 1, 1, 1, 1, 4, 1, 60
    };

    /* h[-1] = 1, h[0] = 30  (numerators of convergents). */
    seq_t h_prev2 = (seq_t)1;   /* h_{-1} */
    seq_t h_prev1 = (seq_t)30;  /* h_0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* index into q is (i-1) mod 16; use bitwise AND for mod 16. */
        unsigned idx = (unsigned)((i - 1) & 15U);
        int coeff = q[idx];
        seq_t temp = ((seq_t)coeff) * h_prev1 + h_prev2;
        h_prev2 = h_prev1;
        h_prev1 = temp;
    }
    return h_prev1;
}
