/*
 * A064990  If A_k denotes the first 3^k terms, then A_0 = 0, A_{k+1} = A_k
 *          A_k B_k, where B_k is obtained from A_k by interchanging 0's and
 *          1's.
 * https://oeis.org/A064990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = parity of the number of 2's in the base‑3 expansion of n
 *            (i.e., a(n) = 1 if the count of digits equal to 2 in the ternary
 *            representation of n is odd, otherwise 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t parity = 0;
    while (n) {
        if ((n % 3) == 2)
            parity ^= 1;
        n /= 3;
    }
    return parity;
}
