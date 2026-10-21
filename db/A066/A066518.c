/*
 * A066518  Anti-divisor class sums of n.
 * https://oeis.org/A066518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 103, i16 n <= 103, i32 n <= 103, i64 n <= 103,
 *            i128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Anti‑divisor class sums of n (OEIS A066518)
 *
 * For each d in [2, n-1] an anti‑divisor of n contributes its class:
 *   -1 if n mod d == (d-1)/2   (d odd)
 *   +1 if n mod d == (d+1)/2   (d odd)
 *   0  if n mod d == d/2       (d even)
 * The return value is the sum of these classes.
 *
 * This implementation uses only integer arithmetic and runs in
 * O(n) time, suitable for n up to at least 10 000 as required.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* Even d can only contribute class 0 (since n mod 2 != 1 for any n).
       Thus we only need to examine odd d. */
    for (idx_t d = 3; d < n; d += 2)
    {
        idx_t rem = n % d;
        idx_t left = d / 2;          /* (d-1)/2 for odd d          */
        if (rem == left)
            sum -= (seq_t)1;
        else if (rem == left + 1)
            sum += (seq_t)1;
    }

    return sum;
}
