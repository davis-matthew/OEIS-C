/*
 * A083107  Diagonal of A083106.
 * https://oeis.org/A083107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A083106(n(n+1)/2) = largest of the n smallest numbers
 *            whose product == 1 mod n (constructive row computation)
 * Range:     u8 n <= 59, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A083107_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int64_t A083107_egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) { *x = 1; *y = 0; return a; }
    int64_t x1, y1;
    int64_t g = A083107_egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

static uint64_t A083107_inv(uint64_t a, uint64_t m)
{
    int64_t x, y;
    A083107_egcd((int64_t)a, (int64_t)m, &x, &y);
    if (x < 0) x += (int64_t)m;
    return (uint64_t)x;
}

seq_t a(idx_t n)
{
    uint64_t r = (uint64_t)n;
    if (r <= 1) return (seq_t)1;

    uint64_t *row = (uint64_t *)malloc((size_t)r * sizeof(uint64_t));
    if (row == NULL) return (seq_t)0;

    uint64_t cnt = 0;
    for (uint64_t i = 1; cnt < r; i++) {
        if (A083107_gcd(i, r) == 1) row[cnt++] = i;
    }

    uint64_t prod = 1, prod_no_last = 1;
    for (uint64_t k = 0; k < r; k++) {
        prod = (uint64_t)(((unsigned __int128)prod * (row[k] % r)) % r);
        if (k == r - 2) prod_no_last = prod;
    }

    uint64_t mx = 0;
    if (prod % r != 1) {
        uint64_t target = A083107_inv(prod_no_last % r, r) % r;
        if (target == 0) target = r;
        uint64_t cand = target;
        for (;;) {
            bool used = false;
            for (uint64_t k = 0; k < r - 1; k++) {
                if (row[k] == cand) { used = true; break; }
            }
            if (!used) break;
            cand += r;
        }
        row[r - 1] = cand;
    }
    for (uint64_t k = 0; k < r; k++)
        if (row[k] > mx) mx = row[k];

    free(row);
    return (seq_t)mx;
}
