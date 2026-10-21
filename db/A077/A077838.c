/*
 * A077838  Expansion of (1-x)/(1-3*x-2*x^2-3*x^3).
 * https://oeis.org/A077838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077838: Expansion of (1‑x)/(1‑3x‑2x²‑3x³)
   Recurrence:  a(0)=1, a(1)=2, a(2)=8,
                a(n)=3·a(n‑1)+2·a(n‑2)+3·a(n‑3)  for n≥3 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    seq_t a0 = (seq_t)1;   /* a(n‑3)   */
    seq_t a1 = (seq_t)2;   /* a(n‑2)   */
    seq_t a2 = (seq_t)8;   /* a(n‑1)   */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i)
    {
        next = (seq_t)3 * a2 + (seq_t)2 * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
