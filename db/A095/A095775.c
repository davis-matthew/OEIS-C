/*
 * A095775  n/2 when 2*A003160(n) = n.
 * https://oeis.org/A095775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k) = n/2 where n is the k-th number with 2*A003160(n) = n;
 *            A003160: b(1)=b(2)=1, b(n) = n - b(b(n-1)) - b(b(n-2)). Computed
 *            by scan
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 934 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *tb = 0;
static uint64_t tb_cap = 0;
static uint64_t tb_max = 0; /* tb[1..tb_max] computed */

static uint64_t *fnd = 0;
static uint64_t fnd_cap = 0;
static uint64_t fnd_cnt = 0;
static uint64_t next_m = 1;

static int tb_ensure(uint64_t N) {
    if (N + 1 > tb_cap) {
        uint64_t nc = tb_cap ? tb_cap * 2 : 1024;
        while (nc < N + 1) nc *= 2;
        uint64_t *p = (uint64_t *)realloc(tb, (size_t)nc * sizeof(uint64_t));
        if (!p) return 0;
        tb = p;
        tb_cap = nc;
    }
    if (tb_max < 2) {
        tb[0] = 0; tb[1] = 1; tb[2] = 1;
        tb_max = 2;
    }
    while (tb_max < N) {
        uint64_t i = tb_max + 1;
        tb[i] = i - tb[tb[i - 1]] - tb[tb[i - 2]];
        tb_max = i;
    }
    return 1;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (fnd_cnt < (uint64_t)n) {
        uint64_t m = next_m;
        if (!tb_ensure(2 * m)) return (seq_t)0;
        if (tb[2 * m] == m) {
            if (fnd_cnt >= fnd_cap) {
                uint64_t nc = fnd_cap ? fnd_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(fnd, (size_t)nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                fnd = p;
                fnd_cap = nc;
            }
            fnd[fnd_cnt++] = m;
        }
        next_m = m + 1;
    }
    return (seq_t)fnd[n - 1];
}
