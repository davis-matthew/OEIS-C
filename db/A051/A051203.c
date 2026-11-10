/*
 * A051203  Least inverse of A005210.
 * https://oeis.org/A051203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A005210(k) = n, where A005210(k) =
 *            |A005210(k-1) + 2*A005210(k-2) - k|, A005210(1) = A005210(2) =
 *            1; found by simulating the recurrence
 * Range:     u8 none, u16 n <= 520, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 78 terms through n = 9138 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *A051203_tab = NULL;
static idx_t A051203_size = 0;
static idx_t A051203_i = 0;
static seq_t A051203_p1, A051203_p2;

static void A051203_reset(idx_t size) {
    if (A051203_tab) free(A051203_tab);
    A051203_tab = (idx_t *)calloc((size_t)size, sizeof(idx_t));
    A051203_size = size;
    A051203_i = 2;
    A051203_p2 = 1; /* A005210(1) */
    A051203_p1 = 1; /* A005210(2) */
    if (size > 1) A051203_tab[1] = 1;
}

seq_t a(idx_t n) {
    if (A051203_tab == NULL || n >= A051203_size || (n >= 1 && 0)) {
        idx_t sz = 2 * n + 1024;
        A051203_reset(sz);
    }
    idx_t guard = 0;
    while (A051203_tab[n] == 0) {
        A051203_i++;
        seq_t ii = (seq_t)A051203_i;
        seq_t sum = A051203_p1 + 2 * A051203_p2;
        seq_t v = (sum >= ii) ? (sum - ii) : (ii - sum);
        A051203_p2 = A051203_p1;
        A051203_p1 = v;
        if (v < (seq_t)A051203_size) {
            idx_t vi = (idx_t)v;
            if (A051203_tab[vi] == 0) A051203_tab[vi] = A051203_i;
        }
        if (++guard > (idx_t)4000000000ULL) return 0;
    }
    return (seq_t)A051203_tab[n];
}
