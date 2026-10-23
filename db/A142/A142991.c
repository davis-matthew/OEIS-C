/*
 * A142991  a(1) = 1, a(2) = 9, a(n+2) = 9*a(n+1)+(n+1)*(n+3)*a(n).
 * https://oeis.org/A142991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=9,
   a(n+2) = 9*a(n+1)+(n+1)*(n+3)*a(n)   for n≥1
   (equivalently, for i≥3: a[i] = 9*a[i-1] + (i-1)*(i+1)*a[i-2]) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)9;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t mult = ((seq_t)(i - 1) * (seq_t)(i + 1));
        seq_t next = (seq_t)9 * prev1 + mult * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
