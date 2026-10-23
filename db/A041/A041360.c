/*
 * A041360  Numerators of continued fraction convergents to sqrt(194).
 * https://oeis.org/A041360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041360:  Numerators of convergents to √194
 *
 *  The partial quotients of √194 are
 *      a0 = 13,
 *  followed by the purely periodic block {1, 12, 1, 26}.
 *
 *  The numerators p[k] of the convergents satisfy
 *      p[-1] = 1,   p[0] = 13,
 *      p[k] = a_k * p[k-1] + p[k-2]   (k ≥ 1)
 *  where a_k is the k‑th partial quotient (a1, a2, …).
 *
 *  This function returns p[n] = a(n) for n ≥ 0.
 *
 *  It works for any seq_t/idx_t chosen by the build:
 *  seq_t may be an unsigned integer of various widths or an
 *  arbitrary‑precision class.  Operations are all on seq_t
 *  (no floating point, no math.h). */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)13;                      /* p[0] */

    static const seq_t period[4] = {
        (seq_t)1,  (seq_t)12,  (seq_t)1,  (seq_t)26
    };                                       /* a1…a4 */

    seq_t prev = (seq_t)1;                    /* p[-1]   */
    seq_t curr = (seq_t)13;                   /* p[0]    */

    idx_t i;
    for (i = 1; i <= n; ++i) {
        idx_t idx = (i - 1) % 4;              /* period index */
        seq_t coeff = period[idx];
        seq_t next = coeff * curr + prev;    /* recurrence */
        prev = curr;
        curr = next;
    }

    return curr;                             /* p[n] */
}
