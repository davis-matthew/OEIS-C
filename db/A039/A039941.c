/*
 * A039941  Alternately add and multiply.
 * https://oeis.org/A039941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 14, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n≥2,
 *   a(n)=a(n-1)+a(n-2) if n even, else a(n-1)*a(n-2) */
seq_t a(idx_t n)
{
    /* handle the two predefined terms */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* iterate from 2 up to n, keeping only the last two values */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur;                /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0)      /* i even */
            cur = prev1 + prev2;
        else                   /* i odd */
            cur = prev1 * prev2;

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
