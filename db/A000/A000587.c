/*
 * A000587  Rao Uppuluri-Carpenter numbers (or complementary Bell numbers):
 *          e.g.f. = exp(1 - exp(x)).
 * https://oeis.org/A000587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, changed
 * Formula:   a(n+1) = -Sum_{i=0..n} C(n,i)*a(i), a(0) = 1, computed with an
 *            Aitken (Bell-type) array: T(0,0) = 1, T(m+1,0) = -T(m,m),
 *            T(m+1,k+1) = T(m+1,k) + T(m,k), and a(n) = T(n,0)
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 18, i64 n <= 29, i128 n <= 48,
 *            big any n
 * Helpers:   t_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/t_grow.h"

static seq_t *t_terms = NULL;   /* t_terms[i] = a(i), i <= t_m */
static seq_t *t_row = NULL;     /* t_row[0..t_m] = row t_m of the array */
static idx_t t_m = 0;
static idx_t t_cap = 0;

seq_t a(idx_t n) {
    if (t_cap == 0) {
        t_cap = 64;
        t_terms = t_grow(NULL, 0, t_cap);
        t_row = t_grow(NULL, 0, t_cap);
        t_terms[0] = 1;
        t_row[0] = 1;
        t_m = 0;
    }
    if (n + 2 > t_cap) {
        idx_t nc = t_cap;
        while (nc < n + 2) nc *= 2;
        t_terms = t_grow(t_terms, t_m + 1, nc);
        t_row = t_grow(t_row, t_m + 1, nc);
        t_cap = nc;
    }
    while (t_m < n) {
        seq_t carry = 0;
        seq_t old;
        idx_t k;
        carry -= t_row[t_m];
        for (k = 0; k <= t_m; k++) {
            old = t_row[k];
            t_row[k] = carry;
            carry += old;
        }
        t_row[t_m + 1] = carry;
        t_m++;
        t_terms[t_m] = t_row[0];
    }
    return t_terms[n];
}
