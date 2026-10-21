/*
 * A083118  Index of zeros in A083116 and A083117.
 * https://oeis.org/A083118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th positive integer that is divisible by 10, 16, or
 *            25
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count how many qualifying numbers have been found so far. */
    idx_t found = 0;

    /* Iterate over all positive integers. */
    for (seq_t x = 1; ; ++x)
    {
        /* Check divisibility by 10, 16, or 25. */
        if (   (x % 10 == 0)
            || (x % 16 == 0)
            || (x % 25 == 0))
        {
            ++found;
            if (found == n)
                return x;          /* The required term is found. */
        }
    }
}
