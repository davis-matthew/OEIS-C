/*
 * A108741  Member r=100 of the family of Chebyshev sequences S_r(n) defined
 *          in A092184.
 * https://oeis.org/A108741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; a(n)=98*a(n-1)-a(n-2)+2 for n>=2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev   = (seq_t)1;  /* a(1) */
    seq_t a_prev2  = (seq_t)0;  /* a(0) */
    seq_t temp;

    for (idx_t i = 2; i <= n; ++i) {
        temp = a_prev;          /* make a copy for multiplication */
        temp *= (seq_t)98;      /* temp = 98 * a(n-1) */
        temp -= a_prev2;        /* temp = 98*a(n-1) - a(n-2) */
        temp += (seq_t)2;       /* temp = 98*a(n-1) - a(n-2) + 2 */
        a_prev2 = a_prev;       /* shift previous values */
        a_prev  = temp;
    }

    return a_prev;
}
