/*
 * A106008  Primes p such that 10*p - 3 and 10*p + 3 are both primes.
 * https://oeis.org/A106008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this sequence is defined by a search: a(n) is the n‑th * prime p
 *            such that 10·p – 3 and 10·p + 3 are both prime. * The
 *            implementation keeps a static memoisation table that is * grown
 *            lazily until it contains n terms
 * Range:     u8 n <= 18, u16 n <= 551, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>
#endif

/* external linkage required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, harmless    */

#ifdef __cplusplus
    /* C++: statically store found terms in a vector<T> (efficient and
     * automatically handles constructors/destructors).                    */
    static std::vector<seq_t> terms;
    static uint64_t candidate = 2;               /* next integer to test           */

    while (terms.size() < n) {
        if (is_prime(candidate)) {
            uint64_t t = candidate * 10ULL;
            if (is_prime(t - 3ULL) && is_prime(t + 3ULL))
                terms.push_back((seq_t)candidate);
        }
        candidate += (candidate == 2) ? 1ULL : 2ULL;
    }
    return terms[n - 1];

#else
    /* Plain C: use a manually sized array allocated with malloc/realloc. */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static uint64_t candidate = 2;
    static size_t capacity = 0;

    if (terms == NULL) {                    /* first call: allocate      */
        capacity = 64;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!terms) return (seq_t)0;
    }

    while (terms_cnt < n) {
        if (is_prime(candidate)) {
            uint64_t t = candidate * 10ULL;
            if (is_prime(t - 3ULL) && is_prime(t + 3ULL)) {
                if (terms_cnt >= capacity) {   /* grow array if needed     */
                    capacity <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                    if (!tmp) {                 /* allocation failed        */
                        free(terms);
                        terms = NULL;
                        return (seq_t)0;
                    }
                    terms = tmp;
                }
                terms[terms_cnt] = (seq_t)candidate;
                ++terms_cnt;
            }
        }
        candidate += (candidate == 2) ? 1ULL : 2ULL;
    }
    return terms[n - 1];
#endif
}
