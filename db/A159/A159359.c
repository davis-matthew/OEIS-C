/*
 * A159359  Number of n X n arrays of squares of integers summing to 5.
 * https://oeis.org/A159359
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2-1)*(n^2+2)*(n^4-11*n^2+48)/120
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* the sequence is defined for n >= 2; return 0 for smaller n */
    if (n < 2) return 0;

    /* convert idx_t to seq_t for all arithmetic */
    seq_t n2 = (seq_t)n * n;                     /* n^2 */
    seq_t n4 = n2 * n2;                          /* n^4 */

    /* the four factors of the numerator */
    seq_t f[4];
    f[0] = n2;                                   /* n^2 */
    f[1] = n2 - 1;                                /* n^2 - 1 */
    f[2] = n2 + 2;                                /* n^2 + 2 */
    f[3] = n4 - (seq_t)11 * n2 + 48;              /* n^4 - 11*n^2 + 48 */

    /* 120 = 2^3 * 3 * 5; divide these prime factors out of the product first
       to avoid overflow in the intermediate multiplication. */
    const int primes[5] = {2, 2, 2, 3, 5};
    for (int pi = 0; pi < 5; ++pi) {
        int p = primes[pi];
        for (int fi = 0; fi < 4; ++fi) {
            if (f[fi] % p == 0) {
                f[fi] /= p;
                break;
            }
        }
    }

    /* multiply the reduced factors */
    seq_t res = f[0] * f[1];
    res *= f[2];
    res *= f[3];

    return res;
}
