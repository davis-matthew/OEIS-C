/*
 * A000152  Number of ways of writing n as a sum of 16 squares.
 * https://oeis.org/A000152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} r8(k)*r8(n-k), where r8 = A000143 (sums of 8
 *            squares): r8(0) = 1, r8(m) = 16*Sum_{d|m} (-1)^(m+d)*d^3 for m
 *            >= 1 (theta_3^16 = (theta_3^8)^2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 515,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int64_t *r8_tab = NULL;
static idx_t r8_cap = 0; /* table holds indices 0..r8_cap-1 */

static void r8_ensure(idx_t n)
{
    idx_t N, d, m;
    if (n < r8_cap)
        return;
    N = n + 1;
    if (N < 2 * r8_cap)
        N = 2 * r8_cap;
    if (N < 64)
        N = 64;
    free(r8_tab);
    r8_tab = (int64_t *)calloc((size_t)N, sizeof(int64_t));
    for (d = 1; d < N; d++) {
        int64_t c = (int64_t)d * (int64_t)d * (int64_t)d;
        for (m = d; m < N; m += d) {
            if ((m + d) & 1)
                r8_tab[m] -= c;
            else
                r8_tab[m] += c;
        }
    }
    for (m = 1; m < N; m++)
        r8_tab[m] *= 16;
    r8_tab[0] = 1;
    r8_cap = N;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    r8_ensure(n);
    for (k = 0; k < n - k; k++) {
        seq_t x = (seq_t)(uint64_t)r8_tab[k];
        seq_t y = (seq_t)(uint64_t)r8_tab[n - k];
        s += 2 * (x * y);
    }
    if (k == n - k) {
        seq_t x = (seq_t)(uint64_t)r8_tab[k];
        s += x * x;
    }
    return s;
}
