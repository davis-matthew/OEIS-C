/*
 * A041696  Numerators of continued fraction convergents to sqrt(368).
 * https://oeis.org/A041696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(368)
 *
 * The sequence is defined by the recurrence
 *   a(0) = 19
 *   a(n) = p_n * a(n-1) + a(n-2)   (n >= 1)
 * where the periodic partial quotients repeat
 *   [5, 2, 5, 38].
 * This follows from the continued‑fraction expansion of √368.
 */
seq_t a(idx_t n)
{
    /* Partial quotients of the continued fraction of sqrt(368)
     * after the initial term a0 = 19. They repeat with period 4. */
    const idx_t period[4] = {5, 2, 5, 38};

    /* Convergents recurrence: h(-2)=0, h(-1)=1 */
    seq_t h_prev2 = 0;   /* h_{k-2}  */
    seq_t h_prev1 = 1;   /* h_{k-1}  */
    seq_t h = 0;         /* current convergent numerator h_k */

    /* Iterate from k = 0 up to n, computing h_k each step */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Determine the k‑th partial quotient a_k */
        seq_t a_k = (k == 0)
                    ? (seq_t)19          /* initial term a0 = floor(sqrt(368)) */
                    : (seq_t)period[(k - 1) % 4];

        /* Recurrence: h_k = a_k * h_{k-1} + h_{k-2} */
        h = a_k * h_prev1 + h_prev2;

        /* If we've reached the requested index, return the result */
        if (k == n)
            return h;

        /* Shift for next iteration */
        h_prev2 = h_prev1;
        h_prev1 = h;
    }

    /* The loop above always returns; this is here to silence
     * compiler warnings about missing return statements. */
    return 0;
}
