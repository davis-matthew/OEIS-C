/*
 * A081008  a(n) = Fibonacci(4n+2) - 1, or Fibonacci(2n)*Lucas(2n+2).
 * https://oeis.org/A081008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4n+2) – 1 (OEIS A081008) Recurrence: a(0) = 0
 *            a(1) = 7 a(2) = 54 a(n) = 8*a(n‑1) – 8*a(n‑2) + a(n‑3) for n ≥ 3
 *            This implementation uses only the last three terms and therefore
 *            works for any seq_t (fixed‑size or arbitrary‑precision) and for
 *            any n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)54;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)7;   /* a(n-2) */
    seq_t a2 = (seq_t)54;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = a2;                       /* ai = a(n-1) */
        ai *= (seq_t)8;                 /* 8*a(n-1) */
        seq_t temp = a1;                /* a(n-2) */
        temp *= (seq_t)8;               /* 8*a(n-2) */
        ai -= temp;                     /* 8*a(n-1) - 8*a(n-2) */
        ai += a0;                        /* + a(n-3) */
        /* shift window: a0 <- a1, a1 <- a2, a2 <- ai */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
