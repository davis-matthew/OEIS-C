/*
 * A133646  a(n)=5*a(n-1)+30*a(n-2) for n>=3, a(0)=1, a(1)=5, a(2)=50 .
 * https://oeis.org/A133646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n‑1) + 30*a(n‑2) for n ≥ 3; a(0) = 1, a(1) = 5, a(2)
 *            = 50
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)50;

    /* Iterate using the recurrence.  Keep only a(n‑2) and a(n‑1). */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)5;   /* a(1) */
    seq_t a_n   = (seq_t)50;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = 5 * a_n + 30 * a_nm1; /* a(i) = 5*a(i‑1) + 30*a(i‑2) */
        a_nm1 = a_n;      /* shift window: previous a(i‑1) becomes a(i‑2) */
        a_n   = next;     /* current becomes new a(i‑1) */
    }
    return a_n;
}
