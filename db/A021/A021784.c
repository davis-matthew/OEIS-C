/*
 * A021784  Expansion of 1/((1-x)(1-4x)(1-5x)(1-11x)).
 * https://oeis.org/A021784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021784: a(n) = 21*a(n‑1) - 139*a(n‑2) + 339*a(n‑3) - 220*a(n‑4)
   Initial values: a(0)=1, a(1)=21, a(2)=302, a(3)=3762
   The function computes the value directly using the recurrence in O(n). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)302;
    if (n == 3) return (seq_t)3762;

    /* Previous four terms (a(n‑4) … a(n‑1)) */
    seq_t a0 = (seq_t)1;    /* a(n‑4) */
    seq_t a1 = (seq_t)21;   /* a(n‑3) */
    seq_t a2 = (seq_t)302;  /* a(n‑2) */
    seq_t a3 = (seq_t)3762; /* a(n‑1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* next = 21*a3 - 139*a2 + 339*a1 - 220*a0 */
        next = a3 * (seq_t)21;
        next = next - a2 * (seq_t)139;
        next = next + a1 * (seq_t)339;
        next = next - a0 * (seq_t)220;

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
