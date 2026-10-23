/*
 * A162751  Write down in binary the n-th positive (odd) integer that is a
 *          palindrome in base 2. Take only the leftmost half of the digits
 *          (including the middle digit if there are an odd number of digits).
 *          a(n) is the decimal equivalent of the result.
 * https://oeis.org/A162751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=a(2)=1; for i>=2, a(2i-1)=2*a(i-1), a(2i)=2*a(i-1)+1
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)1;
    }
    if (n & 1) {                        /* n is odd, n = 2i-1  */
        return 2 * a((n - 1) >> 1);
    } else {                            /* n is even, n = 2i  */
        return 2 * a((n - 2) >> 1) + (seq_t)1;
    }
}
