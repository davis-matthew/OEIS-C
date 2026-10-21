/*
 * A091702  Column 0 of triangle A091700.
 * https://oeis.org/A091702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n‑1) + 2*a(n‑2) – 2*a(n‑3) – a(n‑4)
 * for n > 3, with the initial values:
 * a(0) = 1, a(1) = 2, a(2) = 7, a(3) = 23.
 */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)23;

    /* Use a sliding window of the last four values. */
    seq_t a0 = (seq_t)1;  /* a(n-4)  */
    seq_t a1 = (seq_t)2;  /* a(n-3)  */
    seq_t a2 = (seq_t)7;  /* a(n-2)  */
    seq_t a3 = (seq_t)23; /* a(n-1)  */
    seq_t an = a3;        /* placeholder for a(n) */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        an = 3 * a3 + 2 * a2 - 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
