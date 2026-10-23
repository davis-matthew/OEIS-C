/*
 * A025978  Expansion of 1/((1-2x)(1-4x)(1-8x)(1-11x)).
 * https://oeis.org/A025978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Sequence A025978 : Expansion of 1/((1-2x)(1-4x)(1-8x)(1-11x))
 *
 * Recurrence:   a(0)=1, a(1)=25, a(2)=415, a(3)=5805
 *               a(n) = 25*a(n-1) - 210*a(n-2) + 680*a(n-3) - 704*a(n-4)   (n≥4)
 */
seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)415;
    if (n == 3) return (seq_t)5805;

    /* Sliding window: a0 = a(n‑4), a1 = a(n‑3), a2 = a(n‑2), a3 = a(n‑1) */
    seq_t a0 = (seq_t)1, a1 = (seq_t)25, a2 = (seq_t)415, a3 = (seq_t)5805;
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term using the recurrence */
        a4 = (seq_t)((seq_t)25 * a3
                     - (seq_t)210 * a2
                     + (seq_t)680 * a1
                     - (seq_t)704 * a0);
        /* Advance the sliding window */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4;
    }

    return a3;
}
