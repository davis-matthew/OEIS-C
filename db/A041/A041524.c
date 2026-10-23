/*
 * A041524  Numerators of continued fraction convergents to sqrt(279).
 * https://oeis.org/A041524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041524: numerators of continued fraction convergents to √279.
   Recurrence: let p_{-2}=0, p_{-1}=1.
   Let a₀=16. For k≥1 let a_k follow the period (1,2,2,1,2,2,1,32) repeated.
   Then p_k = a_k * p_{k-1} + p_{k-2}.
   a(n) = p_n. */
seq_t a(idx_t n)
{
    /* n = 0 special case */
    if (n == 0)
        return (seq_t)16;

    /* keep previous two numerators */
    seq_t prevprev = 0; /* p_{i-2} */
    seq_t prev     = 1; /* p_{i-1} */
    seq_t curr     = 0; /* p_i */

    /* periodic part of the continued fraction after a₀ */
    static const unsigned char pattern[8] = {1,2,2,1,2,2,1,32};

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
        {
            /* a₀ = 16 */
            curr = (seq_t)16 * prev + prevprev;   /* = 16 */
        }
        else
        {
            unsigned char a_i = pattern[(i - 1) % 8];
            curr = (seq_t)a_i * prev + prevprev;
        }

        if (i == n)
            return curr;

        /* shift for next step */
        prevprev = prev;
        prev     = curr;
    }

    /* unreachable, but return last computed value to keep compiler happy */
    return curr;
}
