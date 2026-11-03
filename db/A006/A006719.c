/*
 * A006719  a(n) = A000905(n) + 1.
 * https://oeis.org/A006719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000905(n) + 1, where A000905 (Hamilton numbers)
 *            satisfies H(1) = 2, H(m) = 2 + Sum_{k=1..m-1} (-1)^(k+1) *
 *            binomial(H(m-k), k+1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

/* binomial(x, j) for j >= 2, computed multiplicatively */
static seq_t binx(seq_t x, idx_t j)
{
    if (j == 2) {
        if (x < 2) return 0;
        if (x % 2 == 0) return (x / 2) * (x - 1);
        return x * ((x - 1) / 2);
    }
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < j; i++) {
        seq_t f = x - (seq_t)i;
        if (f == 0) return 0;
        r = r * f / ((seq_t)i + 1);
    }
    return r;
}

static seq_t *H = 0;
static idx_t Hlen = 0; /* H[0..Hlen-1] allocated; H[1..Hcnt] valid */
static idx_t Hcnt = 0;

seq_t a(idx_t n)
{
    if (n < 1) n = 1;
    if (n + 1 > Hlen) {
        idx_t nl = (n + 1 > 2 * Hlen) ? n + 1 : 2 * Hlen;
        H = grow_zero(H, Hlen, nl);
        Hlen = nl;
    }
    if (Hcnt == 0) {
        H[1] = 2;
        Hcnt = 1;
    }
    while (Hcnt < n) {
        idx_t m = Hcnt + 1;
        seq_t pos = 2, neg = 0;
        idx_t k;
        for (k = 1; k <= m - 1; k++) {
            seq_t t = binx(H[m - k], k + 1);
            if (k % 2 == 1) pos += t;
            else neg += t;
        }
        H[m] = pos - neg;
        Hcnt = m;
    }
    return H[n] + 1;
}
