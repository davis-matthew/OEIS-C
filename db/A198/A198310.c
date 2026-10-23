/*
 * A198310  Moore lower bound on the order of a (10,g)-cage.
 * https://oeis.org/A198310
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula per OEIS A198310:
 *   a(n) = a(n-1) + 9*a(n-2) – 9*a(n-3)   (n ≥ 6)
 *   a(3) = 11,  a(4) = 20,  a(5) = 101
 */
seq_t a(idx_t n)
{
    /* values below the offset are irrelevant */
    if (n < 3) return 0;

    /* base cases */
    if (n == 3) return 11;
    if (n == 4) return 20;
    if (n == 5) return 101;

    /* iteratively apply the recurrence */
    seq_t prev3 = 11;   /* a(n-3)   */
    seq_t prev2 = 20;   /* a(n-2)   */
    seq_t prev1 = 101;  /* a(n-1)   */
    seq_t cur = 0;

    for (idx_t i = 6; i <= n; ++i) {
        cur  = prev1 + 9 * prev2 - 9 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
