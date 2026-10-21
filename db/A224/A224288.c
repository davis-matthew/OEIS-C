/*
 * A224288  Number of permutations of length n containing exactly 2
 *          occurrences of 123 and 2 occurrences of 132.
 * https://oeis.org/A224288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-11) * (n^4 - 18 n^3 + 219 n^2 - 994 n + 1504)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 25, u64 n <= 52, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t  = (seq_t)n;
    seq_t n2   = n_t * n_t;
    seq_t n3   = n2 * n_t;
    seq_t n4   = n3 * n_t;
    seq_t poly =
        n4
        - (seq_t)18 * n3
        + (seq_t)219 * n2
        - (seq_t)994 * n_t
        + (seq_t)1504;

    if (n > 10) {
        /* multiply by 2^(n-11) -> shift left */
        return poly << (n - 11);
    } else {
        /* divide by 2^(11-n) -> shift right */
        return poly >> (11 - n);
    }
}
