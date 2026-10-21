/*
 * A087705  First integer > n reached under iteration of map x ->
 *          (5/3)*floor(x) when started at n, or -1 if no such integer is ever
 *          reached.
 * https://oeis.org/A087705
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first integer > n reached by iterating x → (5/3)·floor(x)
 *            starting from n, or –1 if none
 * Range:     u8 n <= 9, u16 n <= 283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)-1;          /* n below the OEIS offset – undefined for this problem */

    seq_t m = (seq_t)n;           /* current integer part (floor of the current value) */
    while (m % 3 != 0)            /* look for a multiple of 3 */
    {
        m = (m * 5) / 3;          /* m ← floor((5/3)·m); integer division truncates */
    }
    return (m * 5) / 3;           /* (5/3)·m is integer and is the first integer > n visited */
}
