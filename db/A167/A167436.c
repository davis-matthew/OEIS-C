/*
 * A167436  3rd Fibonacci polynomial evaluated at n^n.
 * https://oeis.org/A167436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(2*n) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle n < 1 gracefully (though offset is 1) */
    if (n == 0) return 0;

    seq_t base = (seq_t)n;       /* n as seq_t */
    idx_t  exp  = 2 * n;         /* exponent fits in idx_t for all call ranges */

    seq_t result = 1;            /* accumulate n^(2n) */

    /* exponentiation by squaring */
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    return result + 1;           /* add one as per OEIS definition */
}
