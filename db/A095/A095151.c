/*
 * A095151  a(n+3) = 3*a(n+2) - 2*a(n+1) + 1 with a(0)=0, a(1)=2.
 * https://oeis.org/A095151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=2;
   for n ≥ 2: a(n)=2*a(n‑1)+n+1
   (equivalent to a(n+3)=3*a(n+2)-2*a(n+1)+1) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    seq_t prev_prev = (seq_t)0; /* a(0) */
    seq_t prev      = (seq_t)2; /* a(1) */
    seq_t curr;

    for (idx_t k = 2; k <= n; ++k) {
        curr = prev * (seq_t)2 + (seq_t)k + (seq_t)1;
        prev_prev = prev;
        prev = curr;
    }
    return prev;
}
