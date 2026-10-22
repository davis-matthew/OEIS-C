/*
 * A005947  Tumbling distance for n-input mappings with 2 steps.
 * https://oeis.org/A005947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*B(n+1) - B(n)), where B = Bell numbers (A000110);
 *            E.g.f.: x*(2*exp(2x)+exp(x))*exp(exp(x)-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *ra = 0, *rb = 0, *bell = 0;
static idx_t cap = 0, nb = 0, rlen = 0;

/* Bell triangle: row r has r+1 entries, first entry is B(r) */
static void ensure(idx_t m)
{
    if (nb == 0) {
        cap = 16;
        ra = alloc_zero(cap);
        rb = alloc_zero(cap);
        bell = alloc_zero(cap);
        ra[0] = 1;
        rlen = 1;
        bell[0] = 1;
        nb = 1;
    }
    while (nb <= m) {
        if (rlen + 1 > cap) {
            idx_t nc = cap * 2;
            ra = grow_zero(ra, cap, nc);
            rb = grow_zero(rb, cap, nc);
            bell = grow_zero(bell, cap, nc);
            cap = nc;
        }
        rb[0] = ra[rlen - 1];
        for (idx_t i = 1; i <= rlen; i++)
            rb[i] = rb[i - 1] + ra[i - 1];
        seq_t *t = ra; ra = rb; rb = t;
        rlen++;
        bell[nb] = ra[0];
        nb++;
    }
}

seq_t a(idx_t n)
{
    ensure(n + 1);
    seq_t b1 = bell[n + 1];
    seq_t b0 = bell[n];
    seq_t t = b1 + b1 - b0;
    return (seq_t)n * t;
}
