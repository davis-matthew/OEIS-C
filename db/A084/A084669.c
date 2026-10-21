/*
 * A084669  Primes which are a concatenation of prime(n) and n.
 * https://oeis.org/A084669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime obtained by concatenating prime(k) and k,
 *            for the smallest k such that the concatenation is prime. The
 *            term is found by iterating over k, concatenating prime(k) and k,
 *            testing the result for primality, and collecting the primes in
 *            increasing order
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   append_candidate, concat_parts, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/append_candidate.h"
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t current_k = 1;

    while (cache_len < (size_t)n) {
        seq_t val = concat_parts(get_prime(current_k), current_k);
        if (is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);
        ++current_k;
    }
    return cache[(size_t)n - 1];
}
