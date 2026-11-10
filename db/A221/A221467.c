/*
 * A221467  The Collatz (3x+1) iteration in A220145 sorted and converted to
 *          decimal.
 * https://oeis.org/A221467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest value of A220145 read as binary. Computed
 *            by building the inverse Collatz tree level by level; each
 *            level's values lie in [2^k, 2^(k+1)-1], so levels are already in
 *            order and only need sorting within a level
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef struct { uint64_t m; uint64_t v; } A221467_node;

static int A221467_cmp(const void *x, const void *y)
{
    uint64_t a = ((const A221467_node *)x)->v;
    uint64_t b = ((const A221467_node *)y)->v;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t cap = 16, len = 1;
    A221467_node *cur = (A221467_node *)malloc(cap * sizeof(A221467_node));
    if (!cur)
        return (seq_t)0;
    cur[0].m = 1;
    cur[0].v = 1;

    idx_t remaining = n;
    int depth = 0;
    seq_t result = (seq_t)0;

    while (1) {
        qsort(cur, len, sizeof(A221467_node), A221467_cmp);
        if ((idx_t)len >= remaining) {
            result = (seq_t)cur[remaining - 1].v;
            break;
        }
        remaining -= (idx_t)len;
        if (++depth >= 62)
            break;

        size_t ncap = len * 2 + 4;
        A221467_node *nxt = (A221467_node *)malloc(ncap * sizeof(A221467_node));
        if (!nxt)
            break;
        size_t nl = 0;
        for (size_t i = 0; i < len; i++) {
            uint64_t m = cur[i].m, v = cur[i].v;
            nxt[nl].m = m * 2;
            nxt[nl].v = v * 2;
            nl++;
            if (m % 6 == 4 && m > 4) {
                uint64_t p = (m - 1) / 3;
                nxt[nl].m = p;
                nxt[nl].v = v * 2 + (p & 1);
                nl++;
            }
        }
        free(cur);
        cur = nxt;
        len = nl;
        (void)cap;
    }
    free(cur);
    return result;
}
