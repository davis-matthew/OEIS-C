/*
 * A061486  Let the number of digits in n be k; a(n) = sum of the products of
 *          the digits of n taken r at a time where r ranges from 1 to k.
 * https://oeis.org/A061486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = Π_{digits d of n} (d+1) - 1
 * Range:     u8 n <= 288, u16 n <= 9761, u32 n <= 9761, u64 n <= 9761,
 *            u128 n <= 9761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of products of all non‑empty subsets of the decimal digits of n. */
seq_t a(idx_t n)
{
    seq_t prod = 1;          /* start with multiplicative identity */
    idx_t t = n;

    /* Handle n == 0 as a special case (digits = {0}) */
    if (t == 0)
    {
        /* (0+1) - 1 == 0 */
        return 0;
    }

    /* Multiply (digit + 1) for each decimal digit of n */
    while (t > 0)
    {
        unsigned digit = (unsigned)(t % 10);
        prod *= (seq_t)(digit + 1);
        t /= 10;
    }

    /* Subtract the empty‑subset product (1) to exclude it */
    return prod - 1;
}
