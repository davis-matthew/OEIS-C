/*
 * A003030  Number of strongly connected digraphs with n labeled nodes.
 * https://oeis.org/A003030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Robinson's recurrence. D(m) = 2^(m(m-1)); c(m) = D(m) -
 *            Sum_{k=1..m-1} binomial(m,k)*c(k)*2^(k(m-k))*D(m-k), c(1) = 1;
 *            a(m) = c(m) + Sum_{k=1..m-1} binomial(m-1,k-1)*a(k)*c(m-k)
 *            (e.g.f.: 1 - exp(-A(x)) = C(x))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   A_grow, a3030_two_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A_grow.h"
#include "../utils/a3030_two_pow.h"

static seq_t *a3030_c = NULL;
static seq_t *a3030_s = NULL;
static seq_t *a3030_row = NULL;
static idx_t a3030_done = 0;
static idx_t a3030_cap = 0;

static void a3030_ensure(idx_t n) {
    if (n + 2 <= a3030_cap) return;
    idx_t nc = a3030_cap ? a3030_cap : 16;
    while (nc < n + 2) nc *= 2;
    a3030_c = A_grow(a3030_c, a3030_cap, nc);
    a3030_s = A_grow(a3030_s, a3030_cap, nc);
    a3030_row = A_grow(a3030_row, a3030_cap, nc);
    if (a3030_cap == 0) {
        a3030_row[0] = 1;
        a3030_c[0] = 0;
        a3030_s[0] = 0;
    }
    a3030_cap = nc;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    a3030_ensure(n);
    while (a3030_done < n) {
        idx_t m = a3030_done + 1;
        idx_t k;
        /* row currently holds binomial(m-1, .) */
        seq_t sum_s = 0;
        for (k = 1; k < m; k++)
            sum_s += a3030_row[k - 1] * a3030_s[k] * a3030_c[m - k];
        /* update row to binomial(m, .) */
        a3030_row[m] = 1;
        for (k = m - 1; k >= 1; k--)
            a3030_row[k] += a3030_row[k - 1];
        seq_t sub = 0;
        for (k = 1; k < m; k++) {
            idx_t r = m - k;
            sub += a3030_row[k] * a3030_c[k] * a3030_two_pow(k * r + r * (r - 1));
        }
        seq_t cm = a3030_two_pow(m * (m - 1)) - sub;
        a3030_c[m] = cm;
        a3030_s[m] = cm + sum_s;
        a3030_done = m;
    }
    return a3030_s[n];
}
