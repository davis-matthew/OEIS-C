/*
 * A087708  First integer > n reached under iteration of map x ->
 *          (5/3)*ceiling(x) when started at n, or -1 if no such integer is
 *          ever reached.
 * https://oeis.org/A087708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first integer > n reached by iterating x -> (5/3) *
 *            ceil(x) starting from x0 = n
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: for n == 0, sequence is undefined; return -1 */
    if (n == 0) return (seq_t)-1;

    seq_t m = (seq_t)n;           /* current ceil(x) */
    for (;;)
    {
        seq_t num = m * 5;        /* numerator of next value: 5 * ceil(x) */

        /* if 5*m is divisible by 3, the next x is an integer */
        if (num % 3 == 0) {
            seq_t val = num / 3;  /* integer reached */
            if (val > (seq_t)n)   /* first integer > n */
                return val;
            /* otherwise, continue (this case should not happen) */
        }

        /* compute ceil(num / 3) = (num + 2) / 3 for positive integers */
        m = (num + 2) / 3;
    }

    /* The following is never reached if the conjecture holds */
    return (seq_t)-1;
}
