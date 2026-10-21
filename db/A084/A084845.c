/*
 * A084845  Numerators of the continued fraction n+1/(n+1/...) [n times].
 * https://oeis.org/A084845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the continued fraction n + 1/(n + 1/( ... ))
 *            (n terms)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let p₁ = n, q₁ = 1.
   For k = 2 .. n:
         p_k = n·p_{k‑1} + q_{k‑1}
         q_k = p_{k‑1}
   The returned value is p_n. */
seq_t a(idx_t n)
{
    /* fraction V₁ = n/1 */
    seq_t p = (seq_t)n;   /* numerator of V₁ */
    seq_t q = (seq_t)1;   /* denominator of V₁ */

    /* iterate the continued fraction n‑1 additional times */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t new_p = n * p + q;  /* numerator of Vᵢ */
        seq_t new_q = p;          /* denominator of Vᵢ (previous numerator) */
        p = new_p;
        q = new_q;
    }

    return p;  /* numerator after n terms */
}
