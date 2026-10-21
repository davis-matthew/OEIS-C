/*
 * A099582  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k, k-1)*4^(n-k-1).
 * https://oeis.org/A099582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A099582
 * Definition:  a(0)=0, a(1)=0, a(2)=1, a(3)=4, a(4)=24
 *                 a(n)=4·a(n‑1)+8·a(n‑2)–16·a(n‑3)–16·a(n‑4)  for n≥5
 * This recurrence follows from the original binomial–power formula.
 */

seq_t a(idx_t n)
{
    /* Base values for n<5 */
    static const seq_t base[5] = {0, 0, 1, 4, 24};

    if (n < 5)
        return base[n];

    /* Keep the last four terms:
       a_n_minus4 = a(n‑4), a_n_minus3 = a(n‑3), a_n_minus2 = a(n‑2), a_n_minus1 = a(n‑1) */
    seq_t a_n_minus4 = base[1];   /* a(1) */
    seq_t a_n_minus3 = base[2];   /* a(2) */
    seq_t a_n_minus2 = base[3];   /* a(3) */
    seq_t a_n_minus1 = base[4];   /* a(4) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 4 * a_n_minus1 + 8 * a_n_minus2
                   - 16 * a_n_minus3 - 16 * a_n_minus4;

        /* Shift the window for the next iteration */
        a_n_minus4 = a_n_minus3;
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = next;
    }

    return a_n_minus1;
}
