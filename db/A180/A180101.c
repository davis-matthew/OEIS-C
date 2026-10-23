/*
 * A180101  a(0)=0, a(1)=1; thereafter a(n) = largest prime factor of sum of
 *          all previous terms.
 * https://oeis.org/A180101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by a(0)=0, a(1)=1; thereafter a(n)=largest prime
 *            factor of the sum a(0)+a(1)+...+a(n-1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool initialized = false;
#if defined(__cplusplus)
    static std::vector<seq_t> seq_vec;
#else
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
#endif
    static idx_t max_computed = 0;
    static uint64_t sum = 0;

    /* initialise once */
    if (!initialized) {
#if defined(__cplusplus)
        seq_vec.reserve(2);
        seq_vec.push_back((seq_t)0);
        seq_vec.push_back((seq_t)1);
#else
        seq_cap = 2;
        seq = (seq_t*)realloc(NULL, seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = 0;
        seq[1] = 1;
#endif
        sum = 1;          /* a(0)+a(1) */
        max_computed = 1;
        initialized = true;
    }

    /* return already known value */
    if (n <= max_computed) {
#if defined(__cplusplus)
        return seq_vec[(size_t)n];
#else
        return seq[(size_t)n];
#endif
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* largest prime factor of the running sum */
        seq_t val = (seq_t)largest_prime_factor((uint64_t)sum);

#if defined(__cplusplus)
        seq_vec.push_back(val);
#else
        /* grow the array if necessary */
        if (seq_cap <= (size_t)i) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 1;
            while (new_cap <= (size_t)i) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[(size_t)i] = val;
#endif
        sum += (uint64_t)val;
    }

    max_computed = n;

#if defined(__cplusplus)
    return seq_vec[(size_t)n];
#else
    return seq[(size_t)n];
#endif
}
