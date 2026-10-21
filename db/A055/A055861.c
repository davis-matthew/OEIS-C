/*
 * A055861  Essentially A053506 but with leading 0 (instead of 1) and offset
 *          0.
 * https://oeis.org/A055861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^(n-1) (with a(0) = 0, a(1) = 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        exp >>= 1;
        if (exp) {
            base = base * base;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    /* For n=0 or 1 the sequence is zero */
    if (n < 2) {
        return 0;
    }
    seq_t base = (seq_t)(n + 1);
    seq_t power = pow_seq_t(base, n - 1);
    return ((seq_t)n) * power;
}
