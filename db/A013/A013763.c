/*
 * A013763  a(n) = 18^(3*n + 2).
 * https://oeis.org/A013763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(3*n + 2) Recurrence: a(0) = 324, a(n) = 5832 * a(n-1)
 *            for n >= 1 (since 18^3 = 5832)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint64(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* a(n) = 324 * 5832^n = 18^2 * (18^3)^n = 18^(3n+2) */
    seq_t pow = pow_uint64(5832, n);
    return pow * 324;
}
