/*
 * A054427  Permutation of natural numbers: maps the fractions A038567/A038566
 *          to the right side (n/m > 1) of Stern-Brocot tree.
 * https://oeis.org/A054427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the rank (1‑based index) of the fraction
 *            A038567(n)/A038566(n) in the breadth‑first traversal of the
 *            right side of the Stern‑Brocot tree
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline void fraction_from_index(idx_t n, idx_t *p, idx_t *q) {
    idx_t target = n;
    for (idx_t num = 1;; ++num) {
        for (idx_t den = 1; den <= num; ++den) {
            if (gcd_idx(num, den) == 1) {
                if (--target == 0) {
                    *p = num;
                    *q = den;
                    return;
                }
            }
        }
    }
}

seq_t a(idx_t n) {
    /* obtain the fraction n-th in A038567/A038566 */
    idx_t p, q;
    fraction_from_index(n, &p, &q);

    /* compute rank in the Stern‑Brocot tree by the
       subtractive Euclidean algorithm */
    seq_t path = 0;
    while (p != q) {
        if (p > q) {
            p -= q;
            path = (path << 1) | 1;   /* R */
        } else {
            q -= p;
            path <<= 1;               /* L */
        }
    }
    return path + 1;
}
