/*
 * A163092  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence derived from the generating function of
 *            A163092. For n≥5: a(n) = 14·a(n‑1) + 14·a(n‑2) + 14·a(n‑3) –
 *            105·a(n‑4). For n=4: a(4) = 1 + 14·a(3) + 14·a(2) + 14·a(1) –
 *            105·a(0). Base values: a(0)=1, a(1)=16, a(2)=240, a(3)=3600
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)240;
    if (n == 3) return (seq_t)3600;

    /* slide window of the last four computed values */
    seq_t v0 = (seq_t)1;     /* a(n‑4) */
    seq_t v1 = (seq_t)16;    /* a(n‑3) */
    seq_t v2 = (seq_t)240;   /* a(n‑2) */
    seq_t v3 = (seq_t)3600;  /* a(n‑1) */
    seq_t val = 0;

    for (idx_t i = 4; i <= n; ++i) {
        if (i == 4) {
            val = (seq_t)1
                  + (seq_t)14 * v3
                  + (seq_t)14 * v2
                  + (seq_t)14 * v1
                  - (seq_t)105 * v0;
        } else {
            val = (seq_t)14 * v3
                  + (seq_t)14 * v2
                  + (seq_t)14 * v1
                  - (seq_t)105 * v0;
        }
        /* shift window forward */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = val;
    }
    return val;
}
