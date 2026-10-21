/*
 * A075677  Reduced Collatz function R applied to the odd integers: a(n) =
 *          R(2n-1), where R(k) = (3k+1)/2^r, with r as large as possible.
 * https://oeis.org/A075677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = odd_part(3*n - 1) (i.e. (3*n - 1) with all factors of 2
 *            removed)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A075677 */
seq_t a(idx_t n)
{
    /* Compute 3*n - 1 as a seq_t value */
    seq_t x = 3 * (seq_t)n - 1;

    /* Remove all factors of 2 (i.e., keep only the odd part) */
    while ((x & 1) == 0)
        x >>= 1;   /* or x /= 2; both are allowed */

    return x;
}
