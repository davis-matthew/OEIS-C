/*
 * A042160  Numerators of continued fraction convergents to sqrt(605).
 * https://oeis.org/A042160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numerators of continued‑fraction convergents to sqrt(605).
   Recurrence:
       p_{-2} = 0,   p_{-1} = 1
       p_n   = a_n * p_{n-1} + p_{n-2}
   where a_n are the partial quotients of sqrt(605) (a_0 = ⌊√605⌋ = 24)
   obtained by the standard quadratic‑surd continued‑fraction algorithm. */
seq_t a(idx_t n)
{
    const idx_t D = 605;            /* the radicand */
    const idx_t a0 = 24;            /* ⌊√605⌋ */

    /* Variables for generating the continued‑fraction coefficients a_n. */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Variables for the numerator recurrence. */
    seq_t p_prevprev = (seq_t)0;    /* p_{-2} */
    seq_t p_prev     = (seq_t)1;    /* p_{-1} */
    seq_t p = 0;

    for (idx_t k = 0; k <= n; ++k) {
        p = (seq_t)a * p_prev + p_prevprev;   /* compute p_k */
        if (k == n)                           /* output the requested term */
            return p;

        /* Produce the next coefficient a_{k+1}. */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* Shift the recurrence state for the next iteration. */
        p_prevprev = p_prev;
        p_prev     = p;
    }

    /* Unreachable, but prevents compiler warnings. */
    return (seq_t)0;
}
