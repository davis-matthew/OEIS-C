/*
 * A041645  Denominators of continued fraction convergents to sqrt(341).
 * https://oeis.org/A041645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 18, u64 n <= 39, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(341)
/* Recurrence: a(0)=1, a(1)=2.
   For n>=2: a(n)=t[n-1]*a(n-1)+a(n-2),
   where t = [2,6,1,8,2,1,2,1,2,8,1,6,2,36] (period 14).
*/
seq_t a(idx_t n)
{
    /* Periodic continued‑fraction terms of sqrt(341). */
    static const int period[14] = {2,6,1,8,2,1,2,1,2,8,1,6,2,36};

    if (n == 0)
        return (seq_t)1;           /* a(0) */

    seq_t prev2 = (seq_t)0;         /* a(-1) */
    seq_t prev1 = (seq_t)1;         /* a(0) */
    seq_t cur   = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        int term = period[(k - 1) % 14];      /* a_k for this step */
        cur = (seq_t)term * prev1 + prev2;   /* a(k) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1; /* a(n) */
}
