/*
 * A016174  Expansion of 1/((1-6*x)*(1-11*x)).
 * https://oeis.org/A016174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 6^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively expand the sequence using only 64‑bit integer
   operations, so it works for all seq_t sizes that can hold the
   result.  The loop is O(n) and does not assume an order of
   calls. */
seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)1;   /* a(0) */
    if (n == (idx_t)0)
        return a_val;

    seq_t pow6  = (seq_t)1;   /* 6^0 */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        pow6 = pow6 * (seq_t)6;          /* pow6 = 6^i   */
        a_val = a_val * (seq_t)11 + pow6;/* a(i) = 11*a(i-1)+6^i */
    }

    return a_val;
}
