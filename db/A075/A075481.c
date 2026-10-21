/*
 * A075481  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 64n+47. Corresponds to selection of every 16th
 *          term from A074474.
 * https://oeis.org/A075481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in the Collatz sequence starting from
 *            64*n+47 until the first term that is strictly less than the
 *            starting value
 * Range:     u8 none, u16 n <= 18, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* starting value is 64*n + 47 (type seq_t to avoid overflow in seq_t build) */
    seq_t start = (seq_t)64 * n + 47;
    seq_t cur   = start;
    seq_t count = 1;              /* count the starting term */

    /* iterate until we reach a value smaller than the starting value */
    while (1)
    {
        /* Collatz step: next = cur/2 if even, else 3*cur + 1 */
        if (cur & 1)             /* odd */
            cur = cur * 3 + 1;
        else                      /* even */
            cur = cur >> 1;       /* cur/2 */

        ++count;
        if (cur < start)
            break;
    }

    return count;
}
