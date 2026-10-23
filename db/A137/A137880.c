/*
 * A137880  Indices k of perfect squares among 17-gonal numbers A051869(k) =
 *          k*(15*k - 13)/2.
 * https://oeis.org/A137880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 482*a(n-2) - a(n-4) - 208 Initial values: a(1) = 1, a(2)
 *            = 49, a(3) = 225, a(4) = 23409
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, safety value */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)49;
    if (n == 3) return (seq_t)225;
    if (n == 4) return (seq_t)23409;

    /* iterate with the 4‑term recurrence */
    seq_t prev4 = (seq_t)1;      /* a(n-4) */
    seq_t prev3 = (seq_t)49;     /* a(n-3) */
    seq_t prev2 = (seq_t)225;    /* a(n-2) */
    seq_t prev1 = (seq_t)23409;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = (seq_t)482 * prev2 - prev4 - (seq_t)208;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* a(n) */
}
