/*
 * A229397  Number of n X 3 0..2 arrays with top left element 0, horizontal
 *          differences mod 3 never 1, vertical differences mod 3 never -1,
 *          and antidiagonal differences never 0.
 * https://oeis.org/A229397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 3*a(n-3) - a(n-4) Initial values:
 *            a(1)=4, a(2)=13, a(3)=44, a(4)=153
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is iteratively computed in O(n) time and uses only
   standard arithmetic operators, so it works for every seq_t
   implementation (fixed‑width integer or arbitrary‑precision class). */
seq_t a(idx_t n)
{
    /* Handle the defined offset; a(1) is the first term. */
    if (n == 0)          /* undefined, return 0 as a graceful value */
        return (seq_t)0;
    if (n == 1)          /* a(1) = 4 */
        return (seq_t)4;
    if (n == 2)          /* a(2) = 13 */
        return (seq_t)13;
    if (n == 3)          /* a(3) = 44 */
        return (seq_t)44;
    if (n == 4)          /* a(4) = 153 */
        return (seq_t)153;

    /* Use a rolling window of the last four values. */
    seq_t a1 = (seq_t)4;   /* a(n-4) */
    seq_t a2 = (seq_t)13;  /* a(n-3) */
    seq_t a3 = (seq_t)44;  /* a(n-2) */
    seq_t a4 = (seq_t)153; /* a(n-1) */
    seq_t an = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        an = 5 * a4 - 6 * a3 + 3 * a2 - a1;
        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
