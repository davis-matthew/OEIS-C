/*
 * A094623  Expansion of g.f. x*(1+10*x)/((1-x)*(1-10*x^2)).
 * https://oeis.org/A094623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Linear recurrence: a(n) = a(n-1) + 10*a(n-2) - 10*a(n-3),
   with initial values a(0)=0, a(1)=1, a(2)=11. */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0u)
        return 0u;
    if (n == 1u)
        return 1u;
    if (n == 2u)
        return 11u;

    /* Use a sliding window of the last three computed terms. */
    seq_t a0 = 0u;   /* a(n-3) */
    seq_t a1 = 1u;   /* a(n-2) */
    seq_t a2 = 11u;  /* a(n-1) */
    seq_t ai;        /* a(n) */

    for (idx_t i = 3u; i <= n; ++i) {
        ai = a2 + a1 * (seq_t)10 - a0 * (seq_t)10;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
