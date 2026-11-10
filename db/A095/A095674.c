/*
 * A095674  Triangle read by rows, formed from product of Pascal's triangle
 *          (A007318) and Aitken's (or Bell's) triangle (A011971).
 * https://oeis.org/A095674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = Sum_{j=k..n} binomial(n,j)*A011971(j,k), i.e. Pascal's
 *            triangle times Aitken's array (as lower-triangular matrices);
 *            A011971(0,0)=1, A011971(n,0)=A011971(n-1,n-1),
 *            A011971(n,k)=A011971(n,k-1)+A011971(n-1,k-1); a(m)=T(r,k) with
 *            m=r*(r+1)/2+k
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 112, u64 n <= 313,
 *            u128 n <= 893, big any n
 * Validated: standard, pass, 71 terms through n = 1149 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t **ait_rows = NULL;
static idx_t ait_cnt = 0;
static idx_t ait_cap = 0;

static void ait_build(idx_t r)
{
    while (ait_cnt <= r) {
        if (ait_cnt >= ait_cap) {
            idx_t nc = ait_cap ? ait_cap * 2 : 64;
            seq_t **t = (seq_t **)realloc(ait_rows, (size_t)nc * sizeof(seq_t *));
            if (!t) abort();
            ait_rows = t;
            ait_cap = nc;
        }
        size_t len = (size_t)ait_cnt + 1;
        seq_t *row = mem_new(seq_t, len);
        if (ait_cnt == 0) {
            row[0] = 1;
        } else {
            seq_t *prev = ait_rows[ait_cnt - 1];
            row[0] = prev[ait_cnt - 1];
            for (size_t i = 1; i < len; i++)
                row[i] = row[i - 1] + prev[i - 1];
        }
        ait_rows[ait_cnt] = row;
        ait_cnt++;
    }
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;

    ait_build(r);

    seq_t *b = mem_new(seq_t, (size_t)r + 1);
    b[0] = 1;
    for (idx_t i = 1; i <= r; i++) {
        for (idx_t j = i; j >= 1; j--)
            b[j] = b[j] + b[j - 1];
    }

    seq_t res = 0;
    for (idx_t j = k; j <= r; j++)
        res += b[j] * ait_rows[j][k];

    mem_free(b);
    return res;
}
