/*
 * A050418  Numbers k such that A050416(k) < A050416(k+1).
 * https://oeis.org/A050418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A050416(k) < A050416(k+1); A050416:
 *            b(1)=b(2)=1, b(m+1)=floor(b(m)/3) if that is not in {0,b(1..m)},
 *            else b(m)+b(m-1). Computed by direct iteration
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *b = NULL;       /* b[i] = A050416(i), i>=1 */
    static size_t nb = 0, capb = 0;
    static size_t *asc = NULL;    /* ascent positions found so far */
    static size_t nasc = 0, capasc = 0;

    if (n == 0) return 0;
    size_t target = (size_t)n;

    if (nb == 0) {
        capb = 64;
        b = mem_resize(seq_t, b, capb);
        if (!b) abort();
        b[0] = 0;
        b[1] = 1;
        b[2] = 1;
        nb = 2;
    }

    while (nasc < target) {
        if (nb + 2 > capb) {
            capb *= 2;
            seq_t *nbuf = mem_resize(seq_t, b, capb);
            if (!nbuf) abort();
            b = nbuf;
        }
        seq_t prev = b[nb];
        seq_t cand = prev / 3;
        bool used = (cand == 0);
        if (!used) {
            for (size_t i = 1; i <= nb; ++i) {
                if (b[i] == cand) { used = true; break; }
            }
        }
        seq_t nxt;
        if (used) nxt = prev + b[nb - 1];
        else nxt = cand;
        b[nb + 1] = nxt;
        if (prev < nxt) {
            if (nasc + 1 > capasc) {
                capasc = capasc ? capasc * 2 : 64;
                size_t *na = (size_t *)realloc(asc, capasc * sizeof(size_t));
                if (!na) abort();
                asc = na;
            }
            asc[nasc++] = nb;
        }
        nb++;
    }
    return (seq_t)asc[target - 1];
}
