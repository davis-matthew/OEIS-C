/*
 * A085903  Expansion of (1 + 2*x^2)/((1 + x)*(1 - 2*x)*(1 - 2*x^2)).
 * https://oeis.org/A085903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n/2)-1)^2 for even n, 2*4^((n-1)/2)-1 for odd n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k;
    seq_t t, res;

    if (n & 1ULL) {                /* odd n: n = 2*k+1  */
        k = (n - 1) >> 1;          /* k = (n-1)/2 */

        /* compute 4^k as repeated left shifts of 2 bits */
        t = 1;
        for (idx_t i = 0; i < k; ++i)
            t <<= 2;               /* t = 4^k */

        res = t * 2ULL - 1ULL;    /* 2*4^k - 1 */
        return res;
    } else {                       /* even n: n = 2*k  */
        k = n >> 1;                /* k = n/2 */

        /* compute 2^k as repeated left shifts of 1 bit */
        t = 1;
        for (idx_t i = 0; i < k; ++i)
            t <<= 1;               /* t = 2^k */

        t -= 1;                    /* t = 2^k - 1 */
        res = t * t;               /* square */
        return res;
    }
}
