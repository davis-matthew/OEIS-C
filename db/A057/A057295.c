/*
 * A057295  Numbers n such that n | 10^n + 9^n + 1.
 * https://oeis.org/A057295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that m | 10^m + 9^m + 1
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache of already found terms */
    static seq_t *terms = NULL;
    static size_t terms_count = 0;
    static size_t terms_cap = 0;
    static idx_t last_checked = 1;

    while (terms_count < n) {
        idx_t m = last_checked;
        seq_t mod = (seq_t)m;
        /* compute 10^m mod m and 9^m mod m */
        seq_t r10 = mod_pow((seq_t)10, (uint64_t)m, mod);
        seq_t r9  = mod_pow((seq_t)9,  (uint64_t)m, mod);

        /* (r10 + r9 + 1) % mod without overflow */
        seq_t sum = r10 + r9;
        if (sum >= mod)
            sum -= mod;
        sum += 1;
        if (sum >= mod)
            sum -= mod;

        if (sum == 0) {
            /* store the qualifying number */
            if (terms_count == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 4 : terms_cap * 2;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp) return 0;           /* allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_count++] = (seq_t)m;
        }
        last_checked = m + 1;
    }
    return terms[n - 1];
}
