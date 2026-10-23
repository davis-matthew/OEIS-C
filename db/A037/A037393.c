/*
 * A037393  Numbers k such that every base-5 digit of k is a base-6 digit of
 *          k.
 * https://oeis.org/A037393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Computes the n‑th integer k such that every base‑5 digit of k is
 *            also a base‑6 digit of k. The sequence is enumerated on the fly
 *            and cached for O(1) random access. The caching array size is
 *            fixed at 200 000 entries, ample for n up to at least 10 000
 * Range:     u8 n <= 30, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mask_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mask_of_digits.h"

/* Maximum number of terms that can be stored in the static cache. */
#define MAX_TERMS 200000

/* Static cache and state. */
static seq_t terms[MAX_TERMS];
static idx_t terms_sz = 0;  /* number of terms currently stored */
static seq_t next_k   = 1;  /* next integer to test */

/* Test whether k satisfies the OEIS condition: every digit occurring
   in the base‑5 representation of k also occurs in its base‑6
   representation. */
static bool good(seq_t k) {
    uint16_t m5 = mask_of_digits(k, 5);
    uint16_t m6 = mask_of_digits(k, 6);
    return (m5 & ~m6) == 0;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The function caches
   previously found terms, so repeated calls with arbitrary n are
   efficient.  The cache size is fixed at MAX_TERMS. */
seq_t a(idx_t n) {
    /* Ensure n is not larger than the cache capacity. */
    if (n > MAX_TERMS) {
        /* If n exceeds the cache size, we still compute the missing
           terms on the fly but do not store them beyond the cache. */
        idx_t target = n;
        while (terms_sz < target) {
            if (good(next_k)) {
                if (terms_sz < MAX_TERMS) {
                    terms[terms_sz++] = next_k;
                }
                /* If the cache is full we skip storing further terms. */
            }
            ++next_k;
        }
        if (n <= terms_sz) {
            return terms[n - 1];
        }
        /* For indices beyond the cache we compute directly by scanning. */
        seq_t k = next_k;
        idx_t count = terms_sz;
        while (count < n) {
            if (good(k)) ++count;
            ++k;
        }
        return k - 1;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_sz < n) {
        if (good(next_k)) {
            terms[terms_sz++] = next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
