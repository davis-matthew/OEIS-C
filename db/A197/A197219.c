/*
 * A197219  a(0) = 2, a(n) = Lucas(phi(n)) for n > 0.
 * https://oeis.org/A197219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(n) = Lucas(phi(n)) for n > 0 (Lucas: L0=2, L1=1)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 52, u64 n <= 96, u128 n <= 190,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"

static seq_t *lucas = NULL;
static idx_t lucas_cap = 0;
static idx_t lucas_len = 0;

static void ensure_lucas(idx_t need)
/* Ensure that lucas[0 .. need] are computed. */
{
    if (lucas_cap <= need) {
        idx_t new_cap = lucas_cap ? lucas_cap * 2 : 4;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(lucas, (size_t)new_cap * sizeof(seq_t));
        lucas = tmp;
        lucas_cap = new_cap;
    }

    if (lucas_len == 0) {
        lucas[0] = (seq_t)2;
        lucas_len = 1;
    }
    if (lucas_len == 1 && need >= 1) {
        lucas[1] = (seq_t)1;
        lucas_len = 2;
    }
    for (idx_t i = lucas_len; i <= need; ++i) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }
    lucas_len = need + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    seq_t phi_val = phi(n);
    idx_t phi = (idx_t)phi_val;   /* φ(n) ≤ n‑1 for n>1 */
    ensure_lucas(phi);
    return lucas[phi];
}
