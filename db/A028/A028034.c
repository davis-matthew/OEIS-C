/*
 * A028034  Expansion of 1/((1-3x)(1-4x)(1-6x)(1-9x)).
 * https://oeis.org/A028034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 22·a(n‑1) − 171·a(n‑2) + 558·a(n‑3) − 648·a(n‑4)
   Initial values: a(0)=1, a(1)=22, a(2)=313, a(3)=3682
   (This follows from the generating function 1/((1-3x)(1-4x)(1-6x)(1-9x)).) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)313;
    if (n == 3) return (seq_t)3682;

    /* Slide a window of the last four values: a0..a3 */
    seq_t a0 = (seq_t)1,   /* a(n-4) */
          a1 = (seq_t)22,  /* a(n-3) */
          a2 = (seq_t)313, /* a(n-2) */
          a3 = (seq_t)3682;/* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)22 * a3
                   - (seq_t)171 * a2
                   + (seq_t)558 * a1
                   - (seq_t)648 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
