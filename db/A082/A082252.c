/*
 * A082252  Concatenation of (3n-2), (3n-1) and 3n divided by 3.
 * https://oeis.org/A082252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ((concat of (3n-2),(3n-1),(3n)))/3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where concat(x,y,z) = x*10^(digits(y)+digits(z)) + y*10^(digits(z)) + z. */

static inline int digit_len(seq_t x) {
    int len = 0;
    if (x == 0) return 1;
    while (x != 0) {
        x /= 10;
        ++len;
    }
    return len;
}

static inline seq_t pow10(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n) {
    seq_t t = 3 * (seq_t)n;
    seq_t a_part = t - 2;
    seq_t b_part = t - 1;
    seq_t c_part = t;

    int len_b = digit_len(b_part);
    int len_c = digit_len(c_part);

    seq_t pow_bc = pow10((idx_t)(len_b + len_c));
    seq_t pow_c  = pow10((idx_t)len_c);

    seq_t concat = a_part * pow_bc + b_part * pow_c + c_part;
    return concat / 3;
}
