/*
 * A139800  a(n) = a(n-1) + a(n-2) + a(n-3) + 2*a(n-4), with a(0) = 1 and a(1)
 *          = a(2) = a(3) = 0.
 * https://oeis.org/A139800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑1)+a(n‑2)+a(n‑3)+2*a(n‑4)
   Initial values: a(0)=1, a(1)=a(2)=a(3)=0 */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n <= 3) return (seq_t)0;

    /* Iterate only the necessary number of steps; O(n) */
    seq_t a0 = (seq_t)1;  /* a(k‑4) */
    seq_t a1 = (seq_t)0;  /* a(k‑3) */
    seq_t a2 = (seq_t)0;  /* a(k‑2) */
    seq_t a3 = (seq_t)0;  /* a(k‑1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = a3 + a2 + a1 + (a0 * (seq_t)2);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
