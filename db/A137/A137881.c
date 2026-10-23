/*
 * A137881  a(n) = sqrt(A137880(n)).
 * https://oeis.org/A137881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=7, a(3)=15, a(4)=153;
   for n>=5: a(n) = 22*a(n-2) - a(n-4)   */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)153;

    /* Sliding window of four successive terms */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)7;   /* a(n-3)   */
    seq_t a2 = (seq_t)15;  /* a(n-2)   */
    seq_t a3 = (seq_t)153; /* a(n-1)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = (seq_t)22 * a2 - a0; /* a(n) = 22*a(n-2) - a(n-4) */
        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3; /* a(n) after loop */
}
