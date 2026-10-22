/*
 * A007993  Poincaré series [or Poincare series] of Lie algebra associated
 *          with a certain braid group.
 * https://oeis.org/A007993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 9*(n-1) + 19*C(n-1,2) + 15*C(n-1,3) + 6*C(n-1,4) +
 *            C(n-1,5)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 217, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t small_binom(idx_t m, idx_t k) {
    seq_t r = 1;
    idx_t i;
    if (m < k) return 0;
    for (i = 0; i < k; i++) {
        unsigned d = (unsigned)(i + 1);
        unsigned g = d;
        seq_t f = (seq_t)(m - i);
        while (g > 1) {
            if (r % g == 0 && d % g == 0) break;
            g--;
        }
        r /= g;
        f /= (d / g);
        r *= f;
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 3;
    result += 9 * (seq_t)m;
    result += 19 * small_binom(m, 2);
    result += 15 * small_binom(m, 3);
    result += 6 * small_binom(m, 4);
    result += small_binom(m, 5);
    return result;
}
