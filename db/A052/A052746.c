/*
 * A052746  a(0) = 0; a(n) = (2*n)^(n-1), n > 0.
 * https://oeis.org/A052746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n)^(n-1) for n>0; a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t base = ((seq_t)n) * 2;
    idx_t exp = n - 1;
    return pow_seq_local(base, exp);
}
