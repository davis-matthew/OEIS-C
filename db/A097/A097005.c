/*
 * A097005  A062401(x) is iterated. Initial value = n!. a(n) is the
 *          path-length of trajectory = count of transients and recurrent
 *          terms, i.e., all distinct states arising in trajectory.
 * https://oeis.org/A097005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of distinct values in the orbit of n! under x ->
 *            phi(sigma(x)) (A062401), found by iterating until a value
 *            repeats
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* phi(sigma(m)) by trial-division factorization on seq_t */
static seq_t A097005_step(seq_t m)
{
    seq_t sg = 1;
    seq_t r = m;
    seq_t p;
    for (p = 2; p * p <= r; p += (p == 2 ? 1 : 2)) {
        if (r % p == 0) {
            seq_t term = 1, pw = 1;
            while (r % p == 0) {
                r /= p;
                pw *= p;
                term += pw;
            }
            sg *= term;
        }
    }
    if (r > 1) sg *= (r + 1);

    /* phi(sg) */
    seq_t phi = sg;
    r = sg;
    for (p = 2; p * p <= r; p += (p == 2 ? 1 : 2)) {
        if (r % p == 0) {
            while (r % p == 0) r /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (r > 1) phi = phi / r * (r - 1);
    return phi;
}

seq_t a(idx_t n)
{
    seq_t x = 1;
    idx_t i;
    for (i = 2; i <= n; i++) x *= (seq_t)i;

    size_t cap = 256, cnt = 0;
    seq_t *vis = mem_new(seq_t, cap);
    for (;;) {
        size_t j;
        int found = 0;
        for (j = 0; j < cnt; j++) {
            if (vis[j] == x) { found = 1; break; }
        }
        if (found) break;
        if (cnt == cap) {
            cap *= 2;
            vis = mem_resize(seq_t, vis, cap);
        }
        vis[cnt++] = x;
        x = A097005_step(x);
    }
    mem_free(vis);
    return (seq_t)cnt;
}
