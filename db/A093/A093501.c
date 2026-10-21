/*
 * A093501  Numbers whose digit permutations are all squarefree numbers.
 * https://oeis.org/A093501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 979, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A093501: Numbers whose digit permutations are all squarefree.
   The function searches numbers in increasing order, testing each one
   for the property, and caches the results.  The test of a number uses
   recursion to generate all distinct permutations of its decimal digits
   and checks each permutation with the helper is_squarefree.  It relies
   on the helper functions:
     - is_squarefree(seq_t x)
     - get_digits(seq_t x, uint8_t *buf, size_t *len)
   provided by the build environment.  */

/* Standard includes */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper prototypes from the build */
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x) */
#include "../utils/get_digits.h"      /* void get_digits(seq_t x, uint8_t *buf, size_t *len) */

static bool perm_sf(const uint8_t *digits, size_t len,
                    unsigned long long used, seq_t val) {
    /* All positions used?  Check squarefree of the full permutation. */
    if (used == ((1ULL << len) - 1ULL))
        return is_squarefree(val);

    unsigned long long mask_next;
    for (size_t i = 0; i < len; ++i) {
        if (!(used & (1ULL << i))) {
            /* Skip duplicate digit in the same recursion depth. */
            bool skip = false;
            for (size_t j = 0; j < i; ++j) {
                if (digits[j] == digits[i] && !(used & (1ULL << j))) {
                    skip = true;
                    break;
                }
            }
            if (skip) continue;

            mask_next = used | (1ULL << i);
            if (!perm_sf(digits, len, mask_next, val * 10 + digits[i]))
                return false;
        }
    }
    return true;
}

static bool all_permutations_squarefree(seq_t x) {
    /* Extract decimal digits of x. */
    uint8_t digits[64];  /* More than enough for any seq_t value. */
    size_t len;
    get_digits(x, digits, &len);

    if (len == 0) return false;   /* should not happen for positive integers */
    return perm_sf(digits, len, 0ULL, 0);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n = 0, harmless */

    /* Persistent cache for found terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static seq_t last_checked = ((seq_t)1);

    /* Grow the cache until it contains at least n terms. */
    while (terms_len < n) {
        if (all_permutations_squarefree(last_checked)) {
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = last_checked;
        }
        ++last_checked;
    }
    return terms[n - 1];
}
