/*
 * A106249  Expansion of (1-x+x^2+x^3)/((1+x)*(1+x^2)*(1-x)^2).
 * https://oeis.org/A106249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n‑1) + a(n‑4) – a(n‑5) (n ≥ 5) Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=2, a(4)=3
 * Range:     u8 n <= 510, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the five known initial values directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)1;
    if (n == 3u) return (seq_t)2;
    if (n == 4u) return (seq_t)3;

    /* Maintain the last five terms of the sequence. */
    seq_t a0 = (seq_t)1;  /* a(n‑5)  */
    seq_t a1 = (seq_t)0;  /* a(n‑4)  */
    seq_t a2 = (seq_t)1;  /* a(n‑3)  */
    seq_t a3 = (seq_t)2;  /* a(n‑2)  */
    seq_t a4 = (seq_t)3;  /* a(n‑1)  */
    seq_t an;

    /* Compute successors up to the requested index. */
    for (idx_t i = 5; i <= n; ++i) {
        an = a4 + a1 - a0;   /* a(i) = a(i‑1) + a(i‑4) – a(i‑5) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
