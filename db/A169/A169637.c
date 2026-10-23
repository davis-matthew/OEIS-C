/*
 * A169637  The number of permutations of the first n elements of the
 *          Hofstadter Q-sequence (A005185), augmented by Q(0)=1.
 * https://oeis.org/A169637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow_seq_large, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = (n+1)! ÷ ∏v (freq(v)!) 
   where freq(v) is the multiplicity of value v in the augmented
   Hofstadter Q‑sequence: 1, Q(1), Q(2), …, Q(n).
   The Hofstadter Q‑sequence satisfies: Q(0)=Q(1)=Q(2)=1,
   and for k≥3, Q(k)=Q(k−Q(k−1))+Q(k−Q(k−2)).
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/pow_seq_large.h"

static idx_t *Q_cache = NULL;      /* stores Q[0 .. k] */
static idx_t  Q_cache_len = 0;     /* current number of entries*/

static void ensure_Q_cache(idx_t n)
{
    if (Q_cache_len > n)        /* already have 0..n inclusive */
        return;

    idx_t new_len = n + 1;
    idx_t *new_cache = (idx_t*)calloc((size_t)new_len, sizeof(idx_t));
    if (!new_cache)            /* allocation failure: give up */
        return;

    /* copy existing data */
    for (idx_t i = 0; i < Q_cache_len; ++i)
        new_cache[i] = Q_cache[i];

    /* base cases */
    if (new_len > 0)  new_cache[0] = 1;
    if (new_len > 1)  new_cache[1] = 1;
    if (new_len > 2)  new_cache[2] = 1;

    /* compute missing values */
    for (idx_t i = (Q_cache_len > 2 ? Q_cache_len : 3);
         i <= n; ++i) {
        idx_t a = i - new_cache[i - 1];
        idx_t b = i - new_cache[i - 2];
        new_cache[i] = new_cache[a] + new_cache[b];
    }

    free(Q_cache);
    Q_cache = new_cache;
    Q_cache_len = new_len;
}

static idx_t factorial_prime_exp(idx_t m, idx_t p)
{
    idx_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* only the added 1 */
        return (seq_t)1;

    /* Prepare the Q sequence from 0..n */
    ensure_Q_cache(n);

    /* Find maximum Q value to size frequency array */
    idx_t max_q = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (Q_cache[i] > max_q)
            max_q = Q_cache[i];

    /* Allocate frequency array for values 0..max_q (value 0 never occurs) */
    size_t freq_size = (size_t)(max_q + 1);
    idx_t *freq = (idx_t*)calloc(freq_size, sizeof(idx_t));
    if (!freq)
        return (seq_t)0;          /* allocation failure */

    /* The added 1 in the augmented sequence */
    ++freq[1];

    /* Count frequencies of Q(1)..Q(n) */
    for (idx_t i = 1; i <= n; ++i)
        ++freq[Q_cache[i]];

    /* Build list of multiplicities > 1 (for denominator) */
    idx_t *mlist = (idx_t*)malloc((size_t)(n + 2) * sizeof(idx_t));
    if (!mlist) {
        free(freq);
        return (seq_t)0;
    }
    size_t mcnt = 0;
    for (idx_t v = 0; v <= max_q; ++v)
        if (freq[v] > 1)
            mlist[mcnt++] = freq[v];

    /* Sieve primes up to n+1 */
    idx_t limit = n + 1;
    bool *is_prime = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) {
        free(freq);
        free(mlist);
        return (seq_t)0;
    }
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Compute result using prime exponents */
    seq_t result = (seq_t)1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;

        idx_t exp = factorial_prime_exp(n + 1, p);
        for (size_t k = 0; k < mcnt; ++k)
            exp -= factorial_prime_exp(mlist[k], p);

        if (exp > 0) {
            seq_t pow_val = pow_seq_large((seq_t)p, (seq_t)exp);
            result = seq_mul(result, pow_val);
        }
    }

    free(is_prime);
    free(freq);
    free(mlist);
    return result;
}
