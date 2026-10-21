/*
 * A078696  a(n+1)=a(n)+a(n-1) if a(n-1) odd, a(n+1)=a(n)+a(n-1)/2 if a(n-1)
 *          even.
 * https://oeis.org/A078696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 106,
 *            u128 n <= 212, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1; for n≥3,
   a(n)=a(n-1)+a(n-2) if a(n-2) odd,
   a(n)=a(n-1)+a(n-2)/2 if a(n-2) even. */
seq_t a(idx_t n)
{
    if (n <= 2)        /* a(1)=a(2)=1 */
        return (seq_t)1;

    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 1;   /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        if (prev2 & (seq_t)1)            /* prev2 odd */
            curr = prev1 + prev2;
        else
            curr = prev1 + (prev2 >> 1); /* prev2 / 2 */

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
