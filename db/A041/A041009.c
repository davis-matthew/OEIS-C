/*
 * A041009  Denominators of continued fraction convergents to sqrt(7).
 * https://oeis.org/A041009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 1 a(1) = 1 a(n) = ( (n % 4 == 0) ? 4 : 1 ) * a(n-1) +
 *            a(n-2) for n ≥ 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th denominator of the continued‑fraction convergents of √7 */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;

    /* k_{-1} = 0, k_0 = 1  (denominators of convergents) */
    seq_t k_prev2 = 0;
    seq_t k_prev1 = 1;
    seq_t k_curr  = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* a_i : partial quotient of √7: 2,1,1,1,4,1,1,1,4,… */
        seq_t a_i = (i % 4 == 0) ? (seq_t)4 : (seq_t)1;

        k_curr = a_i * k_prev1 + k_prev2;

        k_prev2 = k_prev1;
        k_prev1 = k_curr;
    }
    return k_curr;
}
