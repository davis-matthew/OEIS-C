/*
 * A011871  [ n(n-1)/18 ].
 * https://oeis.org/A011871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A011871
 * Formula: a(n) = ⎣(1/3) * Σ_{i=1..n} ⎣i/3⎦⎦
 * (equivalently a(n) = ⎣n(n‑1)/18⎦ for all n≥0)
 * This implementation evaluates the closed form in O(1) time. */

seq_t a(idx_t n)
{
    /* n = 3*q + r  , 0 ≤ r ≤ 2  */
    idx_t q = n / 3;
    idx_t r = n % 3;

    /* Compute q*(q‑1)/2 safely */
    seq_t t = 0;
    if (q > 1) {
        seq_t q_seq = (seq_t)q;
        t = q_seq * (q_seq - 1) / 2;     /* integer division */
    }

    /* Sum of floor(i/3) for i=1..n  */
    seq_t sum = t * 3 + (seq_t)q * (seq_t)(r + 1);

    /* a(n) = floor(sum / 3) */
    return sum / 3;
}
