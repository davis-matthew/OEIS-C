/*
 * A228003  Places of records of A227321.
 * https://oeis.org/A228003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index m (offset 0) of the n-th record of A227321;
 *            computed by search over m, tracking records
 * Range:     u8 n <= 20, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 54 terms through n = 1993 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A228003_pg(uint64_t r, uint64_t k)
{
    if (k == 0) return 0;
    return k * ((r - 2) * k - (r - 4)) / 2;
}

static bool A228003_works(uint64_t r, uint64_t m)
{
    uint64_t k = 0, v;
    while ((v = A228003_pg(r, k)) < m) k++;
    uint64_t d = v - m;
    if (d == 0) return true;
    for (uint64_t j = 1;; j++) {
        uint64_t w = A228003_pg(r, j);
        if (w == d) return true;
        if (w > d) return false;
    }
}

#define A228003_CAP 65536

seq_t a(idx_t n)
{
    static uint64_t rec[A228003_CAP];
    static uint64_t cnt = 0, next_m = 0, R = 0;

    while (cnt < n && cnt < A228003_CAP) {
        uint64_t m = next_m++;
        bool any = false;
        for (uint64_t r = 3; r <= R; r++) {
            if (A228003_works(r, m)) { any = true; break; }
        }
        if (any) continue;
        uint64_t r = (R < 2 ? 3 : R + 1);
        while (!A228003_works(r, m)) r++;
        R = r;
        rec[cnt++] = m;
    }
    return (seq_t)rec[n - 1];
}
