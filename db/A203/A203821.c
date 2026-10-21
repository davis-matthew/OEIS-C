/*
 * A203821  Number of (n+1)X4 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11664*49^(n-1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1 */
    seq_t result = 11664;
    if (n > 1) {
        idx_t exp = n - 1;
        seq_t pow = 1;
        seq_t base = 49;
        while (exp > 0) {
            if (exp & 1) pow *= base;
            base *= base;
            exp >>= 1;
        }
        result *= pow;
    }
    return result;
}
