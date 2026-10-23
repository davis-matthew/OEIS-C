/*
 * A176676  Primes not of the form prime + k^2*2^(k-1).
 * https://oeis.org/A176676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime, realloc_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th prime that cannot be written as
   q + k^2·2^(k‑1) with q prime and k ≥ 1.
   The function searches primes in ascending order and stores those
   that satisfy the condition.  It is cache‑friendly so that calls in
   any order run quickly. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/realloc_safe.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* offset is 1 */

    /* Static cache of qualifying primes.  */
    static seq_t *qual = NULL;
    static size_t qual_cnt = 0;
    static size_t qual_cap = 0;
    static idx_t last_checked = 1;   /* last integer examined */

    /* Ensure we have computed at least n qualifying primes.  */
    while (qual_cnt < (size_t)n) {
        /* Find next prime > last_checked.  */
        idx_t p = last_checked;
        do {
            ++p;
        } while (!is_prime(p));
        last_checked = p;

        /* Test whether p can be written as q + k^2·2^(k‑1).  */
        bool qualifies = true;
        for (uint64_t k = 1; ; ++k) {
            /* t = k^2 * 2^(k‑1).  Compute in 128‑bit to avoid overflow.  */
            unsigned __int128 t = (unsigned __int128)k * k;
            t <<= (k - 1);
            if (t >= (unsigned __int128)p) break;
            idx_t sub = (idx_t)t;
            idx_t q   = p - sub;
            if (is_prime(q)) {
                qualifies = false;
                break;
            }
        }

        if (qualifies) {
            /* Grow storage if necessary.  */
            if (qual_cnt == qual_cap) {
                size_t newcap = qual_cap ? qual_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc_safe(qual, newcap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                qual = tmp;
                qual_cap = newcap;
            }
            qual[qual_cnt++] = (seq_t)p;
        }
    }

    return qual[n - 1];
}
