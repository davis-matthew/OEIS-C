/*
 * A082821  {(Product of terms in row n of A082817) + 1} / n.
 * https://oeis.org/A082821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Product of row n of A082817 + 1) / n, where the rows of
 *            A082817 are built greedily: row k takes the k-1 smallest unused
 *            numbers coprime to k, then the smallest unused x with (product*x
 *            + 1) == 0 (mod k). Simulated row by row
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *used_map = NULL;
static uint64_t used_cap = 0;

static void ensure_cap(uint64_t x)
{
    if (x < used_cap) return;
    uint64_t nc = used_cap ? used_cap : 1024;
    while (nc <= x) nc *= 2;
    unsigned char *p = (unsigned char *)realloc(used_map, (size_t)nc);
    memset(p + used_cap, 0, (size_t)(nc - used_cap));
    used_map = p;
    used_cap = nc;
}

static uint64_t g_u64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t inv_mod_k(uint64_t a, uint64_t m)
{
    int64_t t = 0, nt = 1;
    int64_t r = (int64_t)m, nr = (int64_t)(a % m);
    while (nr != 0) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (int64_t)m;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    if (used_cap) memset(used_map, 0, (size_t)used_cap);
    ensure_cap(16);
    uint64_t *row = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    uint64_t lo = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t pm = 1 % k;
        idx_t cnt = 0;
        uint64_t x = lo;
        while (cnt + 1 < k) {
            ensure_cap(x);
            if (!used_map[x] && g_u64(x, k) == 1) {
                used_map[x] = 1;
                pm = (pm * (x % k)) % k;
                if (k == n) row[cnt] = x;
                cnt++;
            }
            x++;
        }
        uint64_t r;
        if (k == 1) r = 0;
        else r = (k - inv_mod_k(pm, k)) % k;
        x = (r == 0) ? k : r;
        ensure_cap(x);
        while (used_map[x]) { x += k; ensure_cap(x); }
        used_map[x] = 1;
        if (k == n) row[cnt] = x;
        ensure_cap(lo);
        while (used_map[lo]) { lo++; ensure_cap(lo); }
    }
    seq_t P = 1;
    idx_t i;
    for (i = 0; i < n; i++) P *= (seq_t)row[i];
    free(row);
    return (P + 1) / (seq_t)n;
}
