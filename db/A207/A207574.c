/*
 * A207574  Numbers k such that phi(k)+2 divides k+2.
 * https://oeis.org/A207574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that φ(k)+2 divides k+2
 * Range:     u8 n <= 31, u16 n <= 3514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/phi.h"

static seq_t *seq = NULL;       /* cached list of found terms           */
static size_t seq_len = 0;       /* number of terms stored              */
static size_t seq_cap = 0;       /* allocated capacity                  */
static idx_t next_k = 1;         /* next integer to examine             */

/* Ensure that seq contains at least (target_index+1) terms.
   target_index is 0‑based. */
static void ensure_terms(idx_t target_index)
{
    size_t tgt = (size_t)target_index;
    while (seq_len <= tgt) {
        seq_t phi_val = phi(next_k);
        seq_t denom = phi_val + (seq_t)2;
        seq_t num   = (seq_t)next_k + (seq_t)2;

        if (num % denom == (seq_t)0) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 1;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    idx_t idx = n - 1;             /* 0‑based index */
    ensure_terms(idx);
    return seq[idx];
}
