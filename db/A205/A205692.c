/*
 * A205692  The number j such that 6 divides prime(k)-prime(j), where
 *          k(n)=A205691(n).
 * https://oeis.org/A205692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j, the n-th pair (j,k) with j<k, prime(j) == prime(k)
 *            (mod 6), ordered by k then j; computed by search
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t cap = 64, pc = 0;
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t *l1 = (uint64_t *)malloc(cap * sizeof(uint64_t)); /* indices, residue 1 */
    uint64_t *l5 = (uint64_t *)malloc(cap * sizeof(uint64_t)); /* indices, residue 5 */
    size_t c1 = 0, c5 = 0;
    idx_t cum = 0;
    uint64_t cand = 1;

    for (;;) {
        /* next prime */
        for (;;) {
            cand++;
            bool ok = true;
            for (size_t i = 0; i < pc; i++) {
                uint64_t q = pr[i];
                if (q * q > cand) break;
                if (cand % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (pc >= cap) {
            cap <<= 1;
            pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            l1 = (uint64_t *)realloc(l1, cap * sizeof(uint64_t));
            l5 = (uint64_t *)realloc(l5, cap * sizeof(uint64_t));
        }
        pr[pc++] = cand;
        uint64_t k = pc;
        int rem = (int)(cand % 6);
        if (rem == 1 || rem == 5) {
            uint64_t *lst = (rem == 1) ? l1 : l5;
            size_t *cnt = (rem == 1) ? &c1 : &c5;
            idx_t m = (idx_t)(*cnt);
            if (cum + m >= n) {
                uint64_t r = lst[n - cum - 1];
                free(pr); free(l1); free(l5);
                return (seq_t)r;
            }
            cum += m;
            lst[(*cnt)++] = k;
        }
    }
}
