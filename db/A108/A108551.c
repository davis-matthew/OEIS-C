/*
 * A108551  Self-descriptive numbers in various bases represented in base 10.
 * https://oeis.org/A108551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 100 a(2) = 136 a(3) = 1425 for n ≥ 4: let b = n + 3 a(n)
 *            = (b-4)·b^(b-1) + 2·b^(b-2) + b^(b-3) + b^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)100;
    if (n == 2) return (seq_t)136;
    if (n == 3) return (seq_t)1425;
    if (n == 0) return (seq_t)0;   /* undefined offset, fallback */

    idx_t b = n + 3;   /* base */

    seq_t pow_b_b_1 = pow_seq((seq_t)b, b - 1);
    seq_t pow_b_b_2 = pow_seq((seq_t)b, b - 2);
    seq_t pow_b_b_3 = pow_seq((seq_t)b, b - 3);
    seq_t pow_b_3   = pow_seq((seq_t)b, 3);

    seq_t term = (seq_t)(b - 4) * pow_b_b_1
              + (seq_t)2 * pow_b_b_2
              + pow_b_b_3
              + pow_b_3;
    return term;
}
