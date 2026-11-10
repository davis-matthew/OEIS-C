/*
 * A059235  The array in A059219 read by antidiagonals in the direction in
 *          which it was constructed.
 * https://oeis.org/A059235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, nice
 * Formula:   the array of A059219 read by antidiagonals in construction
 *            order. Diagonal d (d>=0) has d+1 terms, the first being 0
 *            (diagonal 0 is just 1). Odd d goes up and even d goes down. Each
 *            new term is the previous term of the diagonal plus the sum of
 *            its row (odd d) or of its column (even d) over the te...
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 123, u64 n <= 302,
 *            u128 n <= 760, big any n
 * Validated: standard, pass, 72 terms through n = 1263 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A059235_vals = NULL;
static idx_t A059235_D = 0; /* diagonals 0..D computed */

static void A059235_build(idx_t D)
{
    size_t total = (size_t)(D + 1) * (size_t)(D + 2) / 2;
    seq_t *rs = mem_new(seq_t, (size_t)D + 1);
    seq_t *cs = mem_new(seq_t, (size_t)D + 1);
    seq_t *v = mem_new(seq_t, total);
    v[0] = (seq_t)1;
    rs[0] = (seq_t)1;
    cs[0] = (seq_t)1;
    for (idx_t d = 1; d <= D; ++d) {
        size_t s = (size_t)d * (size_t)(d + 1) / 2;
        seq_t prev = (seq_t)0;
        v[s] = (seq_t)0;
        for (idx_t k = 1; k <= d; ++k) {
            idx_t r, c;
            seq_t val;
            if (d % 2 == 1) {
                r = d - k; c = k;
                val = prev + rs[r];
            } else {
                r = k; c = d - k;
                val = prev + cs[c];
            }
            rs[r] += val;
            cs[c] += val;
            prev = val;
            v[s + (size_t)k] = val;
        }
    }
    mem_free(rs);
    mem_free(cs);
    if (A059235_vals) mem_free(A059235_vals);
    A059235_vals = v;
    A059235_D = D;
}

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= m) ++d;
    if (A059235_vals == NULL || d > A059235_D) {
        idx_t D = d < 64 ? 64 : d;
        A059235_build(D);
    }
    return A059235_vals[(size_t)m];
}
