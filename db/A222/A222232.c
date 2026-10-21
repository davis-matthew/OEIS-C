/*
 * A222232  In the number n, replace all (decimal) digits '2' with '8' and
 *          vice versa.
 * https://oeis.org/A222232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal‑representation of n with every digit ‘2’ swapped
 *            to ‘8’ and every digit ‘8’ swapped to ‘2’
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage required.  idx_t and seq_t are supplied by the build.  */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for arithmetic. */
    seq_t x = (seq_t)n;

    seq_t result = 0;   /* Value to be returned. */
    seq_t mul    = 1;   /* place value (1,10,100,…) */

    /* Process all decimal digits of x. */
    while (x > 0)
    {
        seq_t d  = x % 10;          /* current least significant digit */
        seq_t nd;                   /* digit after swapping 2↔8 */

        if (d == 2)
            nd = 8;
        else if (d == 8)
            nd = 2;
        else
            nd = d;

        result += nd * mul;         /* add digit to its place */
        mul    *= 10;               /* move to the next place */
        x      /= 10;               /* shift right one decimal digit */
    }

    /* n == 0 is handled automatically (result remains 0). */
    return result;
}
