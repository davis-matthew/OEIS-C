/*
 * A028152  Expansion of 1/((1-4x)(1-7x)(1-10x)(1-11x)).
 * https://oeis.org/A028152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*11^(n+3)-14*10^(n+3)+7^(n+4)-2*4^(n+3))/252
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Linear recurrence: a(n) = 32*a(n‑1) - 369*a(n‑2) + 1798*a(n‑3) - 3080*a(n‑4)
   with a(0)=1, a(1)=32, a(2)=655, a(3)=10950 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)655;
    if (n == 3) return (seq_t)10950;

    /* Working variables for a(n‑4), a(n‑3), a(n‑2), a(n‑1) */
    seq_t a0 = (seq_t)1;      /* a(n‑4) */
    seq_t a1 = (seq_t)32;     /* a(n‑3) */
    seq_t a2 = (seq_t)655;    /* a(n‑2) */
    seq_t a3 = (seq_t)10950;  /* a(n‑1) */
    seq_t an; /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute 32*a3 + 1798*a1  and  369*a2 + 3080*a0  with no underflow */
        seq_t pos = (seq_t)32 * a3 + (seq_t)1798 * a1;
        seq_t neg = (seq_t)369 * a2 + (seq_t)3080 * a0;
        an = pos - neg;  /* guaranteed non‑negative for all n */

        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
