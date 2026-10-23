/*
 * A181935  Curling number of binary expansion of n.
 * https://oeis.org/A181935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4360, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Curling number of binary expansion of n.
   The function computes the largest k such that the binary
   representation of n can be written as XY^k (X may be empty,
   Y a non‑empty string).  It examines all possible block lengths
   l and checks repeated suffixes. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* determine bit length of n */
    idx_t L = 0;
    seq_t tmp = n;
    while (tmp != 0) {
        tmp >>= 1;
        ++L;
    }
    if (L == 0) L = 1;            /* n == 0 has a single “0” bit */

    idx_t best = 1;                /* at least 1 (k≥1 always) */
    size_t bits_per_type = sizeof(seq_t) * 8;

    for (idx_t l = 1; l <= L; ++l) {
        idx_t kmax = L / l;
        if (kmax <= best) continue;   /* cannot beat current best */

        /* compute mask for l bits (avoid shifting by width of type) */
        seq_t mask;
        if (l == bits_per_type) {
            mask = ~((seq_t)0);      /* all bits set */
        } else {
            mask = ((seq_t)1 << l) - 1;
        }

        /* try k from largest possible down to the current best+1 */
        for (idx_t k = kmax; k > best; --k) {
            seq_t Y = n & mask;       /* last l bits of current suffix */
            bool ok = true;
            for (idx_t j = 1; j < k; ++j) {
                if (((n >> (l * j)) & mask) != Y) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                best = k;
                break;                  /* move on to next l */
            }
        }
    }
    return (seq_t)best;
}
