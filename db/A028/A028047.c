/*
 * A028047  Expansion of 1/((1-3x)(1-4x)(1-9x)(1-10x)).
 * https://oeis.org/A028047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*10^(n+3) - 7*9^(n+3) + 7*4^(n+3) - 5*3^(n+3))/210
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=26, a(2)=441, a(3)=6214 */
/*            a(n) = 26*a(n‑1) - 235*a(n‑2) + 858*a(n‑3) - 1080*a(n‑4)   */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)441;
    if (n == 3) return (seq_t)6214;

    /* Iterative computation */
    seq_t a0 = (seq_t)1;     /* a(n‑4) */
    seq_t a1 = (seq_t)26;    /* a(n‑3) */
    seq_t a2 = (seq_t)441;   /* a(n‑2) */
    seq_t a3 = (seq_t)6214;  /* a(n‑1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term */
        seq_t next = (seq_t)26 * a3
                     - (seq_t)235 * a2
                     + (seq_t)858 * a1
                     - (seq_t)1080 * a0;

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
