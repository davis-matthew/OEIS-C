/*
 * A237288  Lexicographically earliest sequence of noncomposite numbers such
 *          that a(n)*n / sum(i=1..n, a(n) ) is strictly increasing.
 * https://oeis.org/A237288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically earliest sequence of non‑composite
 *            numbers (1 and primes) such that rₙ = n·a(n) / Σ_{i=1..n} a(i)
 *            is strictly increasing. Computed by a greedy search: for each
 *            n>1 pick the smallest prime greater than the floor of
 *            ((n-1)*a(n-1)*S) / (n*S-(n-1)*a(n-1)), where S = Σ_{i...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 43, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms: terms[0] = a(1) ... terms[sz-1] = a(sz) */
    static seq_t *terms = NULL;
    static size_t sz = 0;          /* number of terms stored              */
    static seq_t cum_sum = 0;      /* Σ_{i=1..sz} a(i)                    */

    /* offset: a(1) = 1 */
    if (n == 0) return 0;   /* outside offset – not used but safe */

    if (sz == 0) {
        terms = (seq_t*)malloc(sizeof(seq_t));
        if (!terms) exit(1);
        terms[0] = (seq_t)1;   /* a(1) */
        sz = 1;
        cum_sum = (seq_t)1;
    }

    if (n <= sz) return terms[n-1];

    for (idx_t k = (idx_t)(sz + 1); k <= n; ++k) {
        seq_t prev = terms[sz - 1];   /* a(k-1)           */
        seq_t S    = cum_sum;         /* Σ_{i=1}^{k-1}a(i) */

        seq_t numerator   = (seq_t)(k-1) * prev * S;
        seq_t denominator = (seq_t)k * S - (seq_t)(k-1) * prev;

        seq_t threshold = numerator / denominator;   /* floor division   */
        seq_t candidate = threshold + (seq_t)1;
        if (candidate < (seq_t)2) candidate = (seq_t)2;   /* primes start at 2 */

        while (!is_prime_big(candidate))
            candidate += (seq_t)1;

        /* append the candidate */
        terms = (seq_t*)realloc(terms, (sz + 1) * sizeof(seq_t));
        if (!terms) exit(1);
        terms[sz] = candidate;
        ++sz;
        cum_sum += candidate;
    }

    return terms[n-1];
}
