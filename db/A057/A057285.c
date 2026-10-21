/*
 * A057285  Numbers n such that n | 12^n + 11^n + 10^n + 9^n + 8^n + 7^n + 6^n
 *          + 5^n + 4^n.
 * https://oeis.org/A057285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   mul_mod, pow_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer m such that
 *        m divides 12^m + 11^m + 10^m + 9^m + 8^m + 7^m + 6^m
 *        + 5^m + 4^m.  Index 1 corresponds to the first such integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"
#include "../utils/pow_mod.h"

static seq_t *terms   = NULL;   /* stored terms of the sequence   */
static size_t terms_len = 0;    /* number of stored terms         */
static size_t terms_cap = 0;    /* capacity of the array          */
static idx_t  last_tested = 0; /* last integer that has been tested */

/* check if n satisfies the defining condition of the sequence */
static inline bool satisfies(idx_t n)
{
    seq_t mod = (seq_t)n;
    seq_t sum = 0;
    static const int bases[9] = {4,5,6,7,8,9,10,11,12};
    for (int i = 0; i < 9; ++i) {
        seq_t val = pow_mod((seq_t)bases[i], (idx_t)n, mod);
        sum += val;
        if (sum >= mod) sum -= mod;          /* keep sum small   */
    }
    return (sum % mod) == 0;
}

/* append a new term to the stored array, expanding if necessary */
static inline void add_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, return 0 as a safety guard */
    while (terms_len < n) {
        ++last_tested;
        if (satisfies(last_tested))
            add_term((seq_t)last_tested);
    }
    return terms[n-1];
}
