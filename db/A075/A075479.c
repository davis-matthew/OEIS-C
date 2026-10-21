/*
 * A075479  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 64n+31. Corresponds to selection of every 16th
 *          term from A074474.
 * https://oeis.org/A075479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Collatz steps starting from 64*n+31 until the
 *            first term strictly below the start is reached. The sequence
 *            begins with a(0) = 92, a(1) = 14, …
 * Range:     u8 none, u16 n <= 15, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* start value: 64*n + 31, all arithmetic in seq_t */
    seq_t start = ((seq_t)n) * 64 + 31;
    seq_t cur   = start;
    idx_t  steps = 1;                     /* count includes the start term */

    while (cur >= start) {
        if ((cur & 1) == 0)              /* even: divide by 2 */
            cur = cur >> 1;
        else                             /* odd: 3*cur + 1 */
            cur = cur * 3 + 1;
        ++steps;
    }
    return (seq_t)steps;
}
