/*
 * A057714  Composite numbers m = Product p_i^{e_i} such that p_j^{e_j} <
 *          p_k^{e_k} for all p_j < p_k.
 * https://oeis.org/A057714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057714: composite numbers m such that in the factorisation  
   m = ∏ p_i^e_i with p_1 < p_2 < … the powers satisfy  
   p_i^e_i < p_{i+1}^{e_{i+1}} for all i.  
   The implementation searches consecutive integers and caches the
   found terms; it works for any seq_t and idx_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_realloc.h"

static bool is_good_composite(uint64_t m)
{
    /* must be composite with at least two distinct prime factors */
    if (is_prime((idx_t)m))
        return false;

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);
    if (cnt < 2)
        return false;

    /* check strict increasing of prime powers */
    for (size_t i = 0; i + 1 < cnt; ++i) {
        seq_t left  = big_pow((seq_t)pr[i],  (seq_t)ex[i]);
        seq_t right = big_pow((seq_t)pr[i+1], (seq_t)ex[i+1]);
        if (left >= right)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of range, harmless */

    /* static cache of already found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_m = 4; /* numbers to test, skip 1‑3 */

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        uint64_t m = next_m++;
        if (is_good_composite(m)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)m;
        }
    }

    return terms[n - 1];
}
