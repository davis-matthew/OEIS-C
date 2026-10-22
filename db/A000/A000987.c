/*
 * A000987  Number of stochastic matrices of integers.
 * https://oeis.org/A000987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (n-2)*a(n-1) + A000985(n-2), n >= 2,
 *            where A000985: b(0)=1, b(1)=1, b(2)=3, b(3)=11, b(m+1) =
 *            (2m+1)*b(m) - m(m-1)*b(m-1) - m(m-1)*b(m-2) +
 *            m(m-1)(m-2)/2*b(m-3) (from e.g.f. exp(x/2 + x^2/4 +
 *            x^2/(2(1-x))) / sqrt(1-x))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo_a = NULL;
static seq_t *memo_b = NULL;
static idx_t memo_cap = 0;
static idx_t memo_done = 0; /* indices 0..memo_done valid in both arrays */

static void ensure_cap(idx_t n) {
    if (memo_a == NULL) {
        memo_cap = 16;
        memo_a = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo_b = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo_a[0] = 0; memo_a[1] = 1; memo_a[2] = 1; memo_a[3] = 2;
        memo_b[0] = 1; memo_b[1] = 1; memo_b[2] = 3; memo_b[3] = 11;
        memo_done = 3;
    }
    if (n >= memo_cap) {
        idx_t nc = memo_cap;
        while (n >= nc) nc *= 2;
        memo_a = (seq_t *)realloc(memo_a, nc * sizeof(seq_t));
        memo_b = (seq_t *)realloc(memo_b, nc * sizeof(seq_t));
        memo_cap = nc;
    }
}

seq_t a(idx_t n) {
    ensure_cap(n);
    if (n <= memo_done) return memo_a[n];
    for (idx_t i = memo_done + 1; i <= n; i++) {
        idx_t m = i - 1;
        idx_t c1 = 2 * m + 1;
        idx_t c2 = m * (m - 1);
        idx_t c3 = m * (m - 1) * (m - 2) / 2;
        seq_t pos = (seq_t)c1 * memo_b[m] + (seq_t)c3 * memo_b[m - 3];
        seq_t neg = (seq_t)c2 * (memo_b[m - 1] + memo_b[m - 2]);
        memo_b[i] = pos - neg;
        memo_a[i] = (seq_t)(i - 2) * memo_a[i - 1] + memo_b[i - 2];
    }
    memo_done = n;
    return memo_a[n];
}
