/*
 * A169261  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1.
   For 1≤n<29 a(n)=8·7^(n-1).
   For n≥29 a(n)=6·∑_{k=1}^{28} a(n‑k) – 21·a(n‑29). */
seq_t a(idx_t n)
{
    if (n < 29) {
        if (n == 0) return (seq_t)1;
        seq_t val = (seq_t)8;          /* a(1) */
        for (idx_t i = 2; i <= n; ++i)
            val *= (seq_t)7;           /* a(i)=a(i‑1)·7 */
        return val;
    }

    /* Circular buffer keeping last 29 terms */
    seq_t cyc[29];
    cyc[0] = (seq_t)1;
    cyc[1] = (seq_t)8;
    for (idx_t i = 2; i < 29; ++i)
        cyc[i] = cyc[i - 1] * (seq_t)7;   /* a(2)…a(28) */

    for (idx_t i = 29; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 1; j <= 28; ++j)
            sum += cyc[(i - j) % 29];
        sum *= (seq_t)6;
        sum -= (seq_t)21 * cyc[(i - 29) % 29];
        cyc[i % 29] = sum;
    }
    return cyc[n % 29];
}
