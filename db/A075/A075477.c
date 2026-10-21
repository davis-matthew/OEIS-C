/*
 * A075477  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 64n+15. Corresponds to selection of every 16th
 *          term from A074474.
 * https://oeis.org/A075477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of first term < 64*n+15 in Collatz sequence
 *            starting at 64*n+15
 * Range:     u8 n <= 0, u16 n <= 26, u32 n <= 9815, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x0 = (seq_t)n * ((seq_t)64) + ((seq_t)15);
    seq_t cur = x0;
    seq_t steps = (seq_t)1;
    while (cur >= x0) {
        if ((cur & (seq_t)1) == 0)
            cur = cur >> 1;
        else
            cur = cur * (seq_t)3 + (seq_t)1;
        ++steps;
    }
    return steps;
}
