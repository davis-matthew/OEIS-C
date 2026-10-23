/*
 * A042302  Numerators of continued fraction convergents to sqrt(678).
 * https://oeis.org/A042302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1354*a(n-2) - a(n-4), a(0)=26, a(1)=677, a(2)=35230,
 *            a(3)=916657
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four initial terms explicitly */
    if (n == 0u) return (seq_t)26;
    if (n == 1u) return (seq_t)677;
    if (n == 2u) return (seq_t)35230;
    if (n == 3u) return (seq_t)916657;

    /* Iterate the linear recurrence up to n. */
    seq_t prev4 = (seq_t)26;          /* a(n-4) */
    seq_t prev3 = (seq_t)677;         /* a(n-3) */
    seq_t prev2 = (seq_t)35230;       /* a(n-2) */
    seq_t prev1 = (seq_t)916657;      /* a(n-1) */
    seq_t curr;

    /* idx_t is unsigned, so use a loop that counts up to n */
    for (idx_t i = 4; i <= n; ++i) {
        curr = ((seq_t)1354 * prev2) - prev4;
        /* shift the window: a(n-4)=>a(n-3), a(n-3)=>a(n-2), etc. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
