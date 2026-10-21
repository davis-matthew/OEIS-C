/*
 * A095968  Number of tilings of an n X n section of the square lattice with
 *          "ribbon tiles". A ribbon tile is a polyomino which has at most one
 *          square on each diagonal running from northwest to southeast.
 * https://oeis.org/A095968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prod(F(2*i)^2, i=1..n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

seq_t a(idx_t n) {
    /* Compute the product P = ∏_{i=1}^{n} F(2i) */
    seq_t P   = 1;      /* P is initialised to 1            */
    seq_t f0  = 0;      /* F(0)                             */
    seq_t f1  = 1;      /* F(1)                             */

    /* Iterate Fibonacci numbers up to F(2n) */
    for (idx_t k = 2; k <= 2 * n; ++k) {
        /* Compute F(k) = F(k-1) + F(k-2) */
        seq_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;

        /* For even k, incorporate F(k) into the product */
        if ((k & 1ULL) == 0) {
            P = P * f1;
        }
    }

    /* The desired term is the square of the product */
    return P * P;
}
