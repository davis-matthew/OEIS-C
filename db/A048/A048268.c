/*
 * A048268  Smallest palindrome greater than n in bases n and n+1.
 * https://oeis.org/A048268
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2*n^2 + 3*n + 2 for n >= 4 Special cases: a(2) = 6643,
 *            a(3) = 10 (for n < 2 the behavior is undefined)
 * Range:     u8 none, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special cases */
    if (n == 2) return (seq_t)6643;
    if (n == 3) return (seq_t)10;

    /* for n >= 4 use the closed‑form formula */
    if (n < 2) return 0;       /* optional safety guard */

    seq_t nn = (seq_t)n;  /* convert index to seq_t once */
    return 2 * nn * nn + 3 * nn + 2;
}
