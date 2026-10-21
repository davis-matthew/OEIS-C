/*
 * A214996  Power floor-ceiling sequence of 2+sqrt(2).
 * https://oeis.org/A214996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑1) + 2*a(n‑2) – 2*a(n‑3) with initial values a(0) =
 *            3, a(1) = 11, a(2) = 37
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two indices directly. */
    if (n == (idx_t)0) return (seq_t)3;
    if (n == (idx_t)1) return (seq_t)11;

    /* For n >= 2 compute iteratively using the recurrence. */
    seq_t a0 = (seq_t)3;   /* a(n-3)   */
    seq_t a1 = (seq_t)11;  /* a(n-2)   */
    seq_t a2 = (seq_t)37;  /* a(n-1)   */

    /* If n == 2 we already have the value. */
    if (n == (idx_t)2) return a2;

    seq_t a3;  /* current a(n) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        a3 = 3 * a2 + 2 * a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
