/*
 * A091256  Table of lcm(x,y) computed for polynomials over GF(2), where (x,y)
 *          runs as (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),...
 * https://oeis.org/A091256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = lcm over GF(2) of the polynomial representations of the
 *            antidiagonal pair (x,y) with index n (A091256)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   bitlen_seq_t, poly_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/poly_mult.h"

static inline int poly_degree(seq_t a) {
    return (a == 0) ? -1 : (int)bitlen_seq_t(a) - 1;
}

static inline seq_t poly_mod(seq_t a, seq_t b) {
    seq_t r = a;
    int deg_b = poly_degree(b);
    while (r != 0 && poly_degree(r) >= deg_b) {
        int shift = poly_degree(r) - deg_b;
        r ^= b << shift;
    }
    return r;
}

static inline seq_t poly_div(seq_t a, seq_t b) {
    seq_t q = 0;
    int deg_b = poly_degree(b);
    seq_t r = a;
    while (r != 0 && poly_degree(r) >= deg_b) {
        int shift = poly_degree(r) - deg_b;
        q ^= ((seq_t)1 << shift);
        r ^= b << shift;
    }
    return q;
}

static inline seq_t poly_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Antidiagonal enumeration: find x,y such that n indices used */
    idx_t s = 2;                     /* current sum x+y */
    idx_t cumulative = 0;            /* entries before current sum */
    while (true) {
        idx_t row_count = s - 1;     /* number of pairs with sum s */
        if (cumulative + row_count >= n)
            break;
        cumulative += row_count;
        s++;
    }
    idx_t offset = n - cumulative - 1;   /* zero‑based offset in row */
    idx_t x = offset + 1;
    idx_t y = s - 1 - offset;

    seq_t a_poly = (seq_t)x;
    seq_t b_poly = (seq_t)y;
    seq_t g = poly_gcd(a_poly, b_poly);
    seq_t a_over_g = poly_div(a_poly, g);
    seq_t lcm = poly_mult(a_over_g, b_poly);
    return lcm;
}
