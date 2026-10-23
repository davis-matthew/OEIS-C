/*
 * A168648  a(n) = (10*2^n + 2*(-1)^n)/3 for n > 0; a(0) = 1.
 * https://oeis.org/A168648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*2^n + 2*(-1)^n)/3 * Recurrence: a(n) = a(n-1) +
 *            2*a(n-2) for n > 2 * Initial values: a(0)=1, a(1)=6, a(2)=14
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few special cases directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;
    if (n == 2u) return (seq_t)14;

    /* Use the recurrence to compute a(n). */
    seq_t prev2 = (seq_t)6;   /* a(n‑2)  */
    seq_t prev1 = (seq_t)14;  /* a(n‑1)  */
    seq_t curr;

    for (idx_t i = 3u; i <= n; ++i)
    {
        curr  = prev1 + (seq_t)2 * prev2; /* a(i) = a(i-1) + 2*a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr; /* curr holds a(n) after the loop */
}
