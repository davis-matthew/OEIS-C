/*
 * A050099  Numbers k such that A050096(k) > A050096(k+1).
 * https://oeis.org/A050099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050096(k) > A050096(k+1); A050096:
 *            a(1)=1, a(n)=floor(a(n-1)/3) if positive and not yet seen, else
 *            8*a(n-1). Computed by direct simulation with private bignums
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    uint32_t *d;
    size_t len;
} A050099_big;

static A050099_big *A050099_vals = NULL;
static size_t A050099_m = 0, A050099_cap = 0;
static idx_t *A050099_desc = NULL;
static size_t A050099_nd = 0, A050099_dcap = 0;

static A050099_big A050099_div3(const A050099_big *x)
{
    A050099_big r;
    r.d = (uint32_t *)malloc((x->len ? x->len : 1) * sizeof(uint32_t));
    r.len = x->len;
    uint64_t rem = 0;
    for (size_t i = x->len; i-- > 0;) {
        uint64_t cur = (rem << 32) | x->d[i];
        r.d[i] = (uint32_t)(cur / 3);
        rem = cur % 3;
    }
    while (r.len > 0 && r.d[r.len - 1] == 0) r.len--;
    return r;
}

static A050099_big A050099_mul8(const A050099_big *x)
{
    A050099_big r;
    r.d = (uint32_t *)malloc((x->len + 1) * sizeof(uint32_t));
    uint32_t carry = 0;
    for (size_t i = 0; i < x->len; i++) {
        r.d[i] = (x->d[i] << 3) | carry;
        carry = x->d[i] >> 29;
    }
    r.len = x->len;
    if (carry) r.d[r.len++] = carry;
    return r;
}

static bool A050099_seen(const A050099_big *q)
{
    for (size_t i = 0; i < A050099_m; i++) {
        const A050099_big *v = &A050099_vals[i];
        if (v->len != q->len) continue;
        if (q->len == 0 || memcmp(v->d, q->d, q->len * sizeof(uint32_t)) == 0)
            return true;
    }
    return false;
}

static void A050099_push(A050099_big b)
{
    if (A050099_m == A050099_cap) {
        A050099_cap = A050099_cap ? A050099_cap * 2 : 256;
        A050099_vals = (A050099_big *)realloc(A050099_vals, A050099_cap * sizeof(A050099_big));
    }
    A050099_vals[A050099_m++] = b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A050099_m == 0) {
        A050099_big one;
        one.d = (uint32_t *)malloc(sizeof(uint32_t));
        one.d[0] = 1;
        one.len = 1;
        A050099_push(one);
    }
    while (A050099_nd < (size_t)n) {
        A050099_big cur = A050099_vals[A050099_m - 1];
        A050099_big q = A050099_div3(&cur);
        if (q.len > 0 && !A050099_seen(&q)) {
            if (A050099_nd == A050099_dcap) {
                A050099_dcap = A050099_dcap ? A050099_dcap * 2 : 64;
                A050099_desc = (idx_t *)realloc(A050099_desc, A050099_dcap * sizeof(idx_t));
            }
            A050099_desc[A050099_nd++] = (idx_t)A050099_m; /* 1-based index of cur */
            A050099_push(q);
        } else {
            free(q.d);
            A050099_push(A050099_mul8(&cur));
        }
    }
    return (seq_t)A050099_desc[n - 1];
}
