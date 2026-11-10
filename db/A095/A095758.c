/*
 * A095758  Number of A095748-primes in range ]2^n,2^(n+1)].
 * https://oeis.org/A095758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p in ]2^n, 2^(n+1}] (n+1 bits) whose
 *            binary expansion has exactly one matching interior mirrored bit
 *            pair (A095748); computed by enumerating such bit patterns
 *            directly and testing primality
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    uint64_t L = (uint64_t)n + 1;          /* bit length of numbers in range */
    uint64_t k = (L - 2) / 2;              /* interior mirrored pairs */
    if (k == 0) return (seq_t)0;
    bool odd = (L & 1) != 0;
    uint64_t total = (uint64_t)1 << k;
    seq_t count = 0;
    seq_t one = 1;
    seq_t base = (one << (L - 1)) | one;

    for (uint64_t j = 0; j < k; ++j) {
        for (uint64_t mask = 0; mask < total; ++mask) {
            seq_t p = base;
            for (uint64_t i = 1; i <= k; ++i) {
                uint64_t low = (mask >> (i - 1)) & 1;
                uint64_t high = (i == j + 1) ? low : (low ^ 1);
                if (low)  p |= (one << i);
                if (high) p |= (one << (L - 1 - i));
            }
            for (int m = 0; m < (odd ? 2 : 1); ++m) {
                seq_t q = p;
                if (m) q |= (one << ((L - 1) / 2));
                if (is_prime_seq(q)) ++count;
            }
        }
    }
    return count;
}
