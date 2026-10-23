/*
 * A041038  Numerators of continued fraction convergents to sqrt(24).
 * https://oeis.org/A041038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 10*a(n-2) - a(n-4) for n ≥ 4, with a(0)=4, a(1)=5,
 *            a(2)=44, a(3)=49
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases. */
    if (n == 0u) return (seq_t)4;
    if (n == 1u) return (seq_t)5;
    if (n == 2u) return (seq_t)44;
    if (n == 3u) return (seq_t)49;

    /* Sliding window a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a0 = (seq_t)4;   /* a(n‑4) */
    seq_t a1 = (seq_t)5;   /* a(n‑3) */
    seq_t a2 = (seq_t)44;  /* a(n‑2) */
    seq_t a3 = (seq_t)49;  /* a(n‑1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = (seq_t)(( (seq_t)10 * a2) - a0); /* a(n) = 10*a(n‑2) – a(n‑4) */
        /* Shift the window forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
