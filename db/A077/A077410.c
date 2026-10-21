/*
 * A077410  Combined Diophantine Chebyshev sequences A077249 and A077251.
 * https://oeis.org/A077410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A077410
    Recurrence:
        a(0) = 1,  a(1) = 2,  a(2) = 12,  a(3) = 21
        a(n) = 10 * a(n‑2) – a(n‑4)   for n ≥ 4
    (This satisfies the given generating function and all listed terms.) */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)21;

    /* Sliding‑window implementation of the recurrence.
       a0 = a(n‑4), a1 = a(n‑3), a2 = a(n‑2), a3 = a(n‑1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t a2 = (seq_t)12;  /* a(2) */
    seq_t a3 = (seq_t)21;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = (seq_t)10 * a2;  /* 10 * a(n‑2) */
        an -= a0;                  /* minus a(n‑4) */
        /* shift window for next step */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;   /* a(n) is now stored in a3 */
}
