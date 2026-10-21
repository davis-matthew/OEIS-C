/*
 * A087348  a(n) = 10*n^2 - 6*n + 1.
 * https://oeis.org/A087348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*n^2 - 6*n + 1
 *           (OEIS A087348)
 * Description: Direct formula evaluation with 64‑bit arithmetic.
 *   No recursion or memoisation is required; the computation is O(1).
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid accidental 64‑bit overflow on idx_t. */
    seq_t nn = (seq_t)n;

    /* Compute the value using the closed‑form expression. */
    seq_t n2      = nn * nn;        /* n^2 */
    seq_t result  = 10 * n2 - 6 * nn + 1;

    return result;
}
