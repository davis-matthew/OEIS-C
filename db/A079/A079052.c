/*
 * A079052  Recamán variation: a(1) = 1; for n > 1, a(n) = a(n-1)-f(n) if that
 *          number is positive and not already in the sequence, otherwise a(n)
 *          = a(n-1)+f(n), where f(n)=phi(n) (cf. A000010).
 * https://oeis.org/A079052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   ensure_seen_capacity, phi_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán variation: a(1) = 1, a(n) = a(n-1)-φ(n) if that number is positive
   and not already in the sequence, otherwise a(n) = a(n-1)+φ(n).
   φ(n) = Euler's totient function. Assumes a(1) is the first term. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ensure_seen_capacity.h"
#include "../utils/phi_idx.h"

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *seq = NULL;            /* array of sequence terms (a(1) in seq[0]) */
    static size_t seq_cap = 0;           /* capacity of seq array */
    static idx_t seq_len = 0;            /* number of terms computed so far */
    static bool *seen = NULL;            /* bool array: seen[value] == true if value already in the sequence */
    static size_t seen_cap = 0;          /* capacity of seen array */

    if (n == 0) return (seq_t)0;        /* undefined, but safe return */

    /* Initialise if this is the first call */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;              /* a(1) = 1 */
        seq_len = 1;

        seen_cap = 1024;
        seen = (bool *)calloc(seen_cap, sizeof(bool));
        seen[1] = true;                 /* value 1 is used */
    }

    /* Compute terms up to n if needed */
    if (n > seq_len) {
        for (idx_t cur = seq_len + 1; cur <= n; ++cur) {
            seq_t f = phi_idx(cur);     /* φ(cur) */
            seq_t prev = seq[seq_len - 1];
            seq_t cand = prev - f;      /* attempt subtraction */

            bool used = false;
            if (cand > 0) {
                size_t idx = (size_t)cand;
                /* if cand is outside current seen array, expand it */
                if (idx >= seen_cap) {
                    ensure_seen_capacity(&seen_cap, &seen, idx + 1);
                }
                used = seen[idx];
            }

            if (cand > 0 && !used) {
                /* keep cand */
            } else {
                cand = prev + f;         /* fallback to addition */
            }

            /* grow seq array if necessary */
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len] = cand;
            seq_len++;

            /* mark value as seen */
            size_t idx = (size_t)cand;
            if (idx >= seen_cap) {
                ensure_seen_capacity(&seen_cap, &seen, idx + 1);
            }
            seen[idx] = true;
        }
    }

    return seq[n - 1];
}
