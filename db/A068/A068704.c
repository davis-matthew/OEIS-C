/*
 * A068704  a(n) = smallest prime obtained as the concatenation of n^k,
 *          n^(k-1), n^(k-2), ..., n^2, n, 1 for some k >= 1; or 0 if no such
 *          prime exists.
 * https://oeis.org/A068704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, hard, more, nonn
 * Formula:   a(n) = smallest prime obtained by concatenating n^k , n^(k-1) ,
 *            … , n , 1 for some k ≥ 1; 0 if none
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_len, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* not defined for n < 1 */

    seq_t n_seq = (seq_t)n;
    const size_t MAX_K = 200;            /* upper bound for search */

    /* pre-allocate powers up to MAX_K */
    seq_t powers[MAX_K + 1];
    powers[0] = (seq_t)1;
    size_t current_k = 0;

    for (idx_t k = 1; k <= (idx_t)MAX_K; ++k) {
        /* extend the powers array if necessary */
        for (size_t i = current_k + 1; i <= (size_t)k; ++i) {
            powers[i] = powers[i - 1] * n_seq;
        }
        current_k = (size_t)k;

        /* build the concatenation n^k , … , 1 */
        seq_t val = (seq_t)0;
        for (int exp = k; exp >= 0; --exp) {
            seq_t part = powers[exp];
            int d = digit_len(part);
            seq_t powd = pow10((idx_t)d);
            val = val * powd + part;
        }

        /* test for primality */
        if (isPrimeGeneric(val)) {
            return val;   /* first (hence smallest) prime found */
        }
    }

    /* no prime found within the search limit */
    return 0;
}
