/*
 * A091225  Characteristic function of A014580: 1 if the n-th GF(2)[X]
 *          polynomial is irreducible, 0 otherwise.
 * https://oeis.org/A091225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the n-th GF(2)[X] polynomial is irreducible, 0
 *            otherwise
 * Range:     u8 n <= 718, u16 n <= 9544, u32 n <= 9544, u64 n <= 9544,
 *            u128 n <= 9544, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

static idx_t degree_seq(seq_t x) { return bitlen_seq_t(x) - 1; }

/* Returns true if polynomial g divides f over GF(2). */
static bool is_divisible(seq_t f, seq_t g) {
    if (g == 0) return false;
    seq_t r = f;
    idx_t deg_g = degree_seq(g);
    idx_t deg_r;
    while (r != 0) {
        deg_r = degree_seq(r);
        if (deg_r < deg_g) break;
        idx_t shift = deg_r - deg_g;
        r ^= g << shift;
    }
    return r == 0;
}

seq_t a(idx_t n) {
    /* Constants 0 and 1 are not irreducible. */
    if (n < 2) return (seq_t)0;

    /* Find the monic polynomial corresponding to index n. */
    idx_t m = n - 2;                  /* skip the two constants */
    idx_t temp = m + 2;               /* used to locate the degree */
    idx_t d = 0;
    while (temp >> 1) {
        temp >>= 1;
        d++;
    }

    idx_t base = ((idx_t)1 << d);     /* 2^d */
    idx_t offset = m - (base - 2);    /* index within degree‑d polynomials */
    seq_t poly = ((seq_t)1 << d) | (seq_t)offset; /* monic polynomial */

    /* Check for any nontrivial monic divisor of degree <= d/2. */
    idx_t max_k = d / 2;
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t limit = ((idx_t)1 << k);
        for (idx_t lower = 0; lower < limit; ++lower) {
            seq_t g = ((seq_t)1 << k) | (seq_t)lower;
            if (is_divisible(poly, g)) return (seq_t)0;
        }
    }
    return (seq_t)1;
}
