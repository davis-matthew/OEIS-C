/*
 * A084970  Numbers whose smallest prime factor is 13.
 * https://oeis.org/A084970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th integer whose smallest prime factor is 13.
    The sequence is 13 times the n-th integer that has no prime
    factors less than 13 (i.e., not divisible by 2, 3, 5, 7 or 11).   */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* k must be coprime to 2,3,5,7,11 */
        if ((k % 2) && (k % 3) && (k % 5) && (k % 7) && (k % 11))
        {
            ++count;
            if (count == n)
                return (seq_t)13 * k;
        }
    }
    /* unreachable */
}
