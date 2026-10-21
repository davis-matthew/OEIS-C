/*
 * A074473  Dropping time for the 3x+1 problem: for n >= 2, number of
 *          iteration that first becomes smaller than the initial value if
 *          Collatz-function (A006370) is iterated starting at n; a(1)=1 by
 *          convention.
 * https://oeis.org/A074473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + min{ k ≥ 1 | Collatz^k(n) < n } where Collatz is
 *            T(n)=n/2 if n even, 3n+1 if n odd. The first iterate that
 *            becomes smaller than the start is counted as its position
 *            (starting from 1)
 * Range:     u8 n <= 26, u16 n <= 2718, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No other headers or external helpers are required. */

seq_t a(idx_t n)
{
    /* By convention a(1)=1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t curr = (seq_t)n;   /* current value in the Collatz sequence */
    idx_t  step = 1;         /* step counter, counting the starting value */

    /* iterate until we get a value smaller than the start */
    while (curr >= (seq_t)n) {
        /* Collatz step */
        if ((curr & 1) == 0)      /* even */
            curr >>= 1;           /* n/2 */
        else                       /* odd */
            curr = curr * 3 + 1;  /* 3n+1 */
        ++step;
    }
    return (seq_t)step;
}
