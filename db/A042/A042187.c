/*
 * A042187  Denominators of continued fraction convergents to sqrt(618).
 * https://oeis.org/A042187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: denominators of continued‑fraction convergents to sqrt(618)
   Recurrence: let coeffs = {1,6,8,6,1,48}, a period of 6.
   Define p_{-1}=0, p_0=1 and for k ≥ 1,
        p_k = coeffs[(k-1) mod 6] * p_{k-1} + p_{k-2}.
   Then a(n) = p_n (with a(0)=1). */

seq_t a(idx_t n)
{
    static const unsigned coeffs[6] = {1, 6, 8, 6, 1, 48};
    seq_t prev2 = 0;   /* p_{-1} */
    seq_t prev1 = 1;   /* p_0  */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned c = coeffs[(k - 1) % 6];
        seq_t next = (seq_t)c * prev1 + prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
