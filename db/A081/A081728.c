/*
 * A081728  Length of periods of Euler numbers modulo prime(n).
 * https://oeis.org/A081728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)-1 if prime(n) ≡ 3 (mod 4); otherwise a(n) =
 *            (prime(n)-1)/2 (with a(1)=1)
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *prime_cache = NULL;
static idx_t prime_cache_count = 0;
static idx_t prime_cache_capacity = 0;

static void ensure_prime_capacity(idx_t new_capacity)
{
    if (new_capacity <= prime_cache_capacity) return;
    idx_t cap = prime_cache_capacity == 0 ? 32 : prime_cache_capacity;
    while (cap < new_capacity) cap *= 2;
    prime_cache = (idx_t*)realloc(prime_cache, cap * sizeof(idx_t));
    prime_cache_capacity = cap;
}

static void add_prime(idx_t p)
{
    ensure_prime_capacity(prime_cache_count + 1);
    prime_cache[prime_cache_count++] = p;
}

static idx_t nth_prime(idx_t n)
{
    if (n == 0) return 0;              /* n ≥ 1 by contract */
    while (prime_cache_count < n) {
        if (prime_cache_count == 0) {
            add_prime((idx_t)2);
            continue;
        }
        idx_t candidate = prime_cache[prime_cache_count - 1] + 1;
        if ((candidate & (idx_t)1) == 0) candidate++;        /* skip even numbers */
        for (;;) {
            bool is_prime = true;
            for (idx_t i = 0; i < prime_cache_count &&
                 prime_cache[i] * prime_cache[i] <= candidate; ++i) {
                if (candidate % prime_cache[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                add_prime(candidate);
                break;
            }
            candidate += 2;
        }
    }
    return prime_cache[n - 1];
}

seq_t a(idx_t n)
{
    idx_t p = nth_prime(n);
    if (p == (idx_t)2) return (seq_t)1;
    seq_t res = (seq_t)(p - (idx_t)1);
    if (p % (idx_t)4 == (idx_t)3) {
        return res;
    } else {
        return res / (seq_t)2;
    }
}
