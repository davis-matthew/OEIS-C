/*
 * A201112  Primes of the form 3n^3+1.
 * https://oeis.org/A201112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime of the form 3k³+1 (k ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* C++ implementation: use std::vector for arbitrary‑precision seq_t */
#if defined(__cplusplus)
    static std::vector<seq_t> primes;
    static idx_t current_k = 1;          /* k has been processed up to this value */

    if (n <= primes.size())
        return primes[n - 1];

    while (primes.size() < n) {
        ++current_k;                     /* next k to test */
        unsigned long long k = current_k;
        __int128 t = (__int128)3 * k * k * k + 1;  /* 3k³+1 */
        unsigned long long num = (unsigned long long)t;
        seq_t candidate = (seq_t)num;
        if (is_prime_seq(candidate))
            primes.push_back(candidate);
    }
    return primes[n - 1];
#else
    /* C implementation: dynamic array for primitive seq_t */
    static seq_t *primes = NULL;
    static idx_t count = 0;
    static idx_t capacity = 0;
    static idx_t current_k = 1;          /* k has been processed up to this value */

    if (n <= count)
        return primes[n - 1];

    while (count < n) {
        ++current_k;                     /* next k to test */
        unsigned long long k = current_k;
        __int128 t = (__int128)3 * k * k * k + 1;  /* 3k³+1 */
        unsigned long long num = (unsigned long long)t;
        seq_t candidate = (seq_t)num;
        if (is_prime_seq(candidate)) {
            if (count == capacity) {
                capacity = capacity ? capacity * 2 : 16;
                seq_t *tmp = realloc(primes, capacity * sizeof(seq_t));
                if (!tmp) return 0;     /* allocation failure */
                primes = tmp;
            }
            primes[count++] = candidate;
        }
    }
    return primes[n - 1];
#endif
}
