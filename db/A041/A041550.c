/*
 * A041550  Numerators of continued fraction convergents to sqrt(293).
 * https://oeis.org/A041550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 17
 *           a(n) = a_k * p_{k-1} + p_{k-2}  for n >= 1,
 *           where p_k is the numerator of the k-th continued‑fraction convergent
 *           of sqrt(293), a_k = 17 when k = 0, and for k >= 1,
 *           a_k = [8,1,1,8,34] repeated (period 5).
 *
 * The function computes a(n) on demand using the recurrence for convergents.
 */

seq_t a(idx_t n) {
    /* The repeating partial quotients after the initial integer part. */
    static const unsigned int period[5] = {8u, 1u, 1u, 8u, 34u};

    /* Convergents: p_{-2} = 0, p_{-1} = 1. */
    seq_t p_prev2 = 0;      /* p_{k-2} */
    seq_t p_prev1 = 1;      /* p_{k-1} */
    seq_t p = 0;            /* current numerator */

    /* Compute terms up to n. */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)17;          /* a_0 = 17 (integer part of sqrt(293)) */
        } else {
            a_k = (seq_t)period[(k - 1) % 5]; /* periodical coefficients */
        }
        p = a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return p;   /* p = a(n) */
}
