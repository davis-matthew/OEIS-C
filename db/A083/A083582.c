/*
 * A083582  a(n) = (8*2^n-5*(-1)^n)/3.
 * https://oeis.org/A083582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8*2^n - 5*(-1)^n) / 3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(n)=a(n-1)+2*a(n-2) for n≥2 */

seq_t a(idx_t n)
{
    /* Handle the first two values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* Use a simple iterative recurrence.
       prev2 holds a(n-2), prev1 holds a(n-1). */
    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)7;          /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + (prev2 << 1);  /* a(i) = a(i-1) + 2*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
