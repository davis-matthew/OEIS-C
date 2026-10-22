/*
 * A007851  Number of elements w of the Weyl group D(n) such that the roots
 *          sent negative by w span an Abelian subalgebra of the Lie algebra.
 * https://oeis.org/A007851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*C(n)/2 - 1 = C(n) + binomial(2n-1,n-1) - 1, where
 *            C(n) = binomial(2n-1,n-1) - binomial(2n-1,n+1) is the Catalan
 *            number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        /* additive Pascal rows: exact modulo 2^k, so correct whenever the result fits */
        idx_t len, r, j, top;
        seq_t *row, B, cat, res;
        if (n > 300) n = 300; /* no such term fits in 128 bits */
        len = n + 2;
        row = (seq_t *)malloc(sizeof(seq_t) * (size_t)len);
        for (j = 0; j < len; j++) row[j] = 0;
        row[0] = 1;
        for (r = 1; r <= 2 * n - 1; r++) {
            top = r < n + 1 ? r : n + 1;
            for (j = top; j >= 1; j--) row[j] += row[j - 1];
        }
        B = row[n - 1];
        cat = row[n - 1] - row[n + 1];
        res = B + cat - 1;
        free(row);
        return res;
    } else {
        /* B = binomial(2n-1, n-1), computed multiplicatively */
        seq_t B = 1, cat;
        idx_t i;
        for (i = 1; i + 1 <= n; i++) {
            B = B * ((seq_t)n + (seq_t)i) / (seq_t)i;
        }
        cat = (seq_t)2 * B / ((seq_t)n + 1);
        return B + cat - 1;
    }
}
