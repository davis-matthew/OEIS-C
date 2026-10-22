/*
 * A003658  Fundamental discriminants of real quadratic fields; indices of
 *          primitive positive Dirichlet L-series.
 * https://oeis.org/A003658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   fundamental discriminants — squarefree d with d ≡ 1 (mod 4), or
 *            4*d with d ≡ 2,3 (mod 4) and d squarefree
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_squarefree(seq_t n) {
    seq_t i;
    for (i = 2; i <= n / i; i++) {
        if (n % (i * i) == 0) {
            return false;
        }
    }
    return true;
}

static bool is_fundamental_discriminant(seq_t k) {
    seq_t r = k % 4;
    if (r == 1) {
        return is_squarefree(k);
    } else if (r == 0) {
        seq_t d = k / 4;
        seq_t d_mod = d % 4;
        if ((d_mod == 2 || d_mod == 3) && is_squarefree(d)) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    while (count < n) {
        k++;
        if (is_fundamental_discriminant(k)) {
            count++;
        }
    }
    return k;
}
