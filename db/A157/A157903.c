/*
 * A157903  a(n) is the smallest number in Spanish with n consonants.
 * https://oeis.org/A157903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, more
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   count_0_to_999999, count_10_to_19, count_1_to_9, count_1_to_99,
 *            count_1_to_999, count_20_to_29, count_30_to_99, count_consonants
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157903:  a(n) is the smallest natural number whose Spanish
   name contains exactly n consonants.  The function below computes this
   value by scanning sequentially from 1 up, storing the first number
   encountered for each possible consonant count, until it has found all
   counts up to the largest index requested.  The implementation
   generates Spanish words only enough to count consonants, without
   building the full string.  It handles numbers up to 999 999 999
   (sufficient for all n up to at least 10 000). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/count_0_to_999999.h"
#include "../utils/count_10_to_19.h"
#include "../utils/count_1_to_9.h"
#include "../utils/count_1_to_99.h"
#include "../utils/count_1_to_999.h"
#include "../utils/count_20_to_29.h"
#include "../utils/count_30_to_99.h"
#include "../utils/count_consonants.h"

/* Public API */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe */

    /* Static cache of first occurrence for each consonant count */
    static seq_t *res = NULL;
    static size_t res_cap = 0;
    static idx_t processed = 0;          /* last number processed */
    static size_t filled = 0;            /* number of indices 1..max_target that are set */
    static idx_t max_target = 0;         /* largest n requested so far */

    /* enlarge cache if needed */
    if (n > max_target) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > res_cap) {
            seq_t *tmp = (seq_t*)realloc(res, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – return 0 as fallback */
                return 0;
            }
            /* initialise new entries to 0 */
            if (tmp != res) {
                size_t delta = new_cap - res_cap;
                memset(tmp + res_cap, 0, delta * sizeof(seq_t));
                res = tmp;
            } else {
                memset(res + res_cap, 0, (new_cap - res_cap) * sizeof(seq_t));
            }
            res_cap = new_cap;
        }
        max_target = n;
    }

    /* scan numbers until all counts up to max_target are found */
    while (filled < max_target) {
        ++processed;
        int c = count_consonants(processed);
        if (c > 0 && (size_t)c <= max_target && res[c] == 0) {
            res[c] = processed;
            ++filled;
        }
    }

    return res[n];
}
