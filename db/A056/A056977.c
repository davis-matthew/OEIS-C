/*
 * A056977  Number of blocks of {0, 1, 1} in binary expansion of n.
 * https://oeis.org/A056977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = a(n), a(2n+1) = a(n) + [n>1 and n≡1 (mod 4)] Initial
 *            values: a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    while (n > 1) {
        if ((n & 1ULL) == 0) {           /* n is even: a(n) = a(n/2) */
            n >>= 1;
        } else {                         /* n is odd: n = 2k+1 */
            n = (n - 1) >> 1;            /* k = (n-1)/2 */
            if (n > 1 && (n & 3ULL) == 1) /* add 1 if k≡1 (mod 4) and k>1 */
                res += 1;
        }
    }
    /* for n = 1, res is already 0, matching a(1) */
    return res;
}
