/*
 * A040771  Continued fraction for sqrt(800).
 * https://oeis.org/A040771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(800) with offset 0 */
/* a(0)=28, thereafter the period 3,1,1,13,1,1,3,56 repeats. */

seq_t a(idx_t n) {
    /* special case: first term */
    if (n == 0) return (seq_t)28;

    /* constant parameters for sqrt(800) */
    const seq_t D = (seq_t)800;
    const seq_t a0 = (seq_t)28;

    /* iterative continued‑fraction generation */
    uint64_t m = 0, d = 1, a = (uint64_t)a0;

    /* compute a(n) */
    for (idx_t i = 1; i <= n; ++i) {
        m = d * a - m;
        /* (D - m*m) is divisible by d, safe for unsigned arithmetic */
        d = (uint64_t)((D - (seq_t)(m * m)) / (seq_t)d);
        a = (uint64_t)((a0 + (seq_t)m) / (seq_t)d);
    }

    return (seq_t)a;
}
