/*
 * A083314  a(n) = (2*4^n-(3^n-1))/2.
 * https://oeis.org/A083314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*4^n-(3^n-1))/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast binary exponentiation for arbitrary‑precision seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1U) result *= cur;
        exp >>= 1U;
        if (!exp) break;
        cur *= cur;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow4 = pow_seq_t(4, n);      /* 4^n */
    seq_t pow3 = pow_seq_t(3, n);      /* 3^n */

    /* a(n) = (2*4^n - 3^n + 1)/2 */
    seq_t numerator = pow4 * 2 - pow3 + 1;
    return numerator / 2;
}
