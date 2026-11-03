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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer m such that
 *        m divides 12^m + 11^m + 10^m + 9^m + 8^m + 7^m + 6^m
 *        + 5^m + 4^m.  Index 1 corresponds to the first such integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* multiply a and b modulo mod, using the seq_t type’s operators   */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (a * b) % mod;
}

/* modular exponentiation: base^exp mod mod, with seq_t arithmetic */
static inline seq_t pow_mod(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 1) return 0;
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mul_mod(result, b, mod);
        exp >>= 1;
        if (exp) b = mul_mod(b, b, mod);
    }
    return result;
}

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
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
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
