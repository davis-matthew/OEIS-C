/*
 * A006842  Triangle read by rows: row n gives numerators of Farey series of
 *          order n.
 * https://oeis.org/A006842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, frac, tabf
 * Formula:   row k lists the numerators of the Farey series F_k (length 1 +
 *            Sum_{i=1..k} phi(i)); the terms are produced by the next-term
 *            rule: after a/b, c/d comes (t*c - a)/(t*d - b) with t = floor((k
 *            + b)/d), starting from 0/1, 1/k
 * Range:     u8 n <= 9934, u16 n <= 9934, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t totient_u64(uint64_t m) {
    uint64_t r = m, x = m, p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            r -= r / p;
        }
    }
    if (x > 1) r -= r / x;
    return r;
}

seq_t a(idx_t n) {
    uint64_t k = 1;
    uint64_t len = 2;          /* |F_1| = 2 */
    uint64_t before = 0;       /* number of terms in rows 1..k-1 */
    uint64_t pos, i;
    uint64_t pa, pb, pc, pd;

    while (before + len < (uint64_t)n) {
        before += len;
        k++;
        len += totient_u64(k);
    }
    pos = (uint64_t)n - before;   /* 1-based position within row k */

    if (pos == 1) return (seq_t)0;

    pa = 0; pb = 1; pc = 1; pd = k;
    for (i = 2; i < pos; i++) {
        uint64_t t = (k + pb) / pd;
        uint64_t e = t * pc - pa;
        uint64_t f = t * pd - pb;
        pa = pc; pb = pd;
        pc = e; pd = f;
    }
    return (seq_t)pc;
}
