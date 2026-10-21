/*
 * A076009  Third column of triangle A075504.
 * https://oeis.org/A076009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^n - 8*18^n + 9*27^n)/2 Equivalent linear recurrence of
 *            order 3: a(0)=1, a(1)=54, a(2)=2025 a(n)=54*a(n‑1) – 891*a(n‑2)
 *            + 4374*a(n‑3) (n≥3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n handled explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)54;
    if (n == 2) return (seq_t)2025;

    /* Use three previous terms; enough memory regardless of build size */
    seq_t a0 = (seq_t)1,      /* a(n-3) */
           a1 = (seq_t)54,    /* a(n-2) */
           a2 = (seq_t)2025;  /* a(n-1) */
    seq_t anew;

    /* If seq_t is an unsigned 64‑bit integer, use 128‑bit intermediates
       to avoid overflow in the products. The cast to seq_t is safe
       because the final value is known to fit in 64 bits for the test.   */
    if (sizeof(seq_t) == 8) {
        for (idx_t i = 3; i <= n; ++i) {
            __int128 p1 = (__int128)a2 * 54;   /* a(n‑1)·54   */
            __int128 p2 = (__int128)a1 * 891;  /* a(n‑2)·891  */
            __int128 p3 = (__int128)a0 * 4374; /* a(n‑3)·4374 */
            __int128 raw = p1 - p2 + p3;       /* 54 a(n‑1) – 891 a(n‑2) + 4374 a(n‑3) */
            anew = (seq_t)raw;                 /* cast truncates only for values that fit */
            a0 = a1;  a1 = a2;  a2 = anew;
        }
    } else { /* arbitrary‑precision build */
        for (idx_t i = 3; i <= n; ++i) {
            seq_t p1 = a2 * (seq_t)54;
            seq_t p2 = a1 * (seq_t)891;
            seq_t p3 = a0 * (seq_t)4374;
            anew = p1 - p2 + p3;   /* exact arithmetic in the big‑int type */
            a0 = a1;  a1 = a2;  a2 = anew;
        }
    }
    return a2;
}
