/*
 * A183651  Number of (n+1) X 9 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n + 2040*4^n + 55980*3^n + 446580*2^n + 1448520
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Compute 5^n, 4^n, 3^n, 2^n using fast exponentiation */
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p3 = pow_seq((seq_t)3, n);
    seq_t p2 = pow_seq((seq_t)2, n);

    /* Evaluate the closed‑form expression */
    return (seq_t)5 * p5
         + (seq_t)2040 * p4
         + (seq_t)55980 * p3
         + (seq_t)446580 * p2
         + (seq_t)1448520;
}
