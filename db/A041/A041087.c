/*
 * A041087  Denominators of continued fraction convergents to sqrt(51).
 * https://oeis.org/A041087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   a(n) = 100*a(n-2) - a(n-4) with a(0)=1, a(1)=7, a(2)=99,
 *            a(3)=700
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)7;
    if (n == 2u) return (seq_t)99;
    if (n == 3u) return (seq_t)700;

    seq_t prev0 = (seq_t)1;   /* a(n-4)  */
    seq_t prev1 = (seq_t)7;   /* a(n-3)  */
    seq_t prev2 = (seq_t)99;  /* a(n-2)  */
    seq_t prev3 = (seq_t)700; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (seq_t)100 * prev2 - prev0; /* a(i) */
        /* shift the previous terms */
        prev0 = prev1;
        prev1 = prev2;
        prev2 = prev3;
        prev3 = val;
    }

    return prev3; /* a(n) */
}
