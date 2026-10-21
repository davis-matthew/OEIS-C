/*
 * A096842  Sigma applied to A096841 produces these repdigits:
 *          a[n]=A000203[A096841(n)].
 * https://oeis.org/A096842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sigma of the n‑th integer m such that sigma(m) is a
 *            repdigit
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   seq_to_str, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n) {
    /* Static tables holding the discovered terms and the current candidate. */
    static bool initialized = false;
    static idx_t candidate = 1;          /* candidate n to test next        */
    static seq_t *terms = NULL;          /* array of found sigma values     */
    static size_t terms_len = 0;         /* current number of stored terms  */
    static size_t terms_cap = 0;         /* allocated capacity for terms    */

    /* Initialise storage. The first term corresponds to candidate = 1. */
    if (!initialized) {
        initialized = true;
        terms_cap = 1024;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        /* Since n starts at 1 and the first qualifying n is 1, we
           immediately add sigma(1)=1 as the first term. */
        seq_t first_sigma = sigma_of(1);
        terms[0] = first_sigma;
        terms_len = 1;
    }

    /* Produce terms until we have at least n of them. */
    while (terms_len < n) {
        candidate++;  /* Next integer to examine. */
        seq_t sigma_val = sigma_of(candidate);

        /* Check whether sigma_val is a repdigit. */
        bool is_rep = true;
        if (sigma_val <= ULLONG_MAX) {
            unsigned long long v = (unsigned long long)sigma_val;
            int d = v % 10;
            v /= 10;
            while (v > 0) {
                if (v % 10 != d) { is_rep = false; break; }
                v /= 10;
            }
        } else {
            char buf[1024];
            size_t len;
            seq_to_str(sigma_val, buf, &len);
            for (size_t i = 1; i < len; ++i) {
                if (buf[i] != buf[0]) { is_rep = false; break; }
            }
        }

        if (is_rep) {
            /* Grow the array if necessary. */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = sigma_val;
        }
    }

    return terms[n - 1];
}
