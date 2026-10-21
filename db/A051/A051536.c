/*
 * A051536  a(n) = least common multiple of {1, 4, 7, 10, 13 ..., 3n+1}
 *          (A016777).
 * https://oeis.org/A051536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nice, nonn
 * Formula:   a(n) = lcm(1, 4, 7, ..., 3*n + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (A051536 – least common multiple of the arithmetic progression 3*i+1) */

/* This implementation works for any seq_t/idx_t combination that supports
 * unsigned integer arithmetic operators + - * / % and the usual
 * assignment. It does not rely on external libraries and does not use
 * any of the helper functions listed in the problem statement. */

static inline seq_t gcd_seq_t(seq_t a, seq_t b) {
    /* Euclidean algorithm for unsigned integers. */
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t l = 1;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t term = 3 * (seq_t)i + 1;   /* 3*i+1 fits in seq_t */
        seq_t g = gcd_seq_t(l, term);
        l = (l / g) * term;              /* avoid overflow by dividing first */
    }
    return l;
}
