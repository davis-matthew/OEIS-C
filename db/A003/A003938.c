/*
 * A003938  Order of (usually) simple Chevalley group B_2(q), q = prime power.
 * https://oeis.org/A003938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^4 * (q^2 - 1) * (q^4 - 1) / (q is odd ? 2 : 1), where q
 *            = A000961(n+1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *prime_powers_cache = NULL;
static idx_t prime_powers_cached_count = 0;

static void ensure_prime_powers(idx_t n) {
    if (prime_powers_cached_count > n) return;
    
    idx_t need = n + 100;
    if (need < 1000) need = 1000;
    
    uint64_t limit = need * 20;
    if (limit > 500000) limit = 500000;
    
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;
    
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    bool *is_pp = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(is_pp, false, (limit + 1) * sizeof(bool));
    is_pp[1] = true;
    
    for (uint64_t p = 2; p <= limit; p++) {
        if (is_prime[p]) {
            uint64_t pk = p;
            while (pk <= limit) {
                is_pp[pk] = true;
                if (pk > limit / p) break;
                pk *= p;
            }
        }
    }
    
    idx_t count = 0;
    for (uint64_t i = 1; i <= limit; i++) {
        if (is_pp[i]) count++;
    }
    
    uint64_t *temp = (uint64_t *)malloc(count * sizeof(uint64_t));
    idx_t idx = 0;
    for (uint64_t i = 1; i <= limit && idx < count; i++) {
        if (is_pp[i]) {
            temp[idx++] = i;
        }
    }
    
    free(is_prime);
    free(is_pp);
    if (prime_powers_cache) free(prime_powers_cache);
    prime_powers_cache = temp;
    prime_powers_cached_count = count;
}

seq_t a(idx_t n) {
    ensure_prime_powers(n);
    uint64_t q = prime_powers_cache[n];
    
    seq_t q_seq = (seq_t)q;
    seq_t q2 = q_seq * q_seq;
    seq_t q4 = q2 * q2;
    
    seq_t result = q4 * (q2 - 1) * (q4 - 1);
    
    if (q & 1) {
        result /= 2;
    }
    
    return result;
}
