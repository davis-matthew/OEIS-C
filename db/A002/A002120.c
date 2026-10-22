/*
 * A002120  a(1) = 0, a(2) = -2; for n > 2, a(n) + a(n-2) - a(n-3) - a(n-5) -
 *          ... - a(n-p) = (-1)^(n+1)*n if n is prime, otherwise = 0, where p
 *          = largest prime < n.
 * https://oeis.org/A002120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1)*n*A010051(n) + Sum_{k=1..n-1}
 *            (-1)^(n-k+1)*A010051(n-k)*a(k)
 * Range:     i8 n <= 24, i16 n <= 55, i32 n <= 122, i64 n <= 254,
 *            i128 n <= 513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


static bool *is_prime_arr = NULL;
static idx_t sieve_len = 0;

static void sieve_of_eratosthenes(idx_t n) {
    if (sieve_len > n) return;
    
    idx_t new_len = (n < 100) ? 100 : n;
    bool *new_sieve = (bool *)malloc((new_len + 1) * sizeof(bool));
    
    if (is_prime_arr != NULL) {
        free(is_prime_arr);
    }
    
    memset(new_sieve, 1, (new_len + 1) * sizeof(bool));
    new_sieve[0] = new_sieve[1] = false;
    
    for (idx_t i = 2; i * i <= new_len; i++) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= new_len; j += i) {
                new_sieve[j] = false;
            }
        }
    }
    
    is_prime_arr = new_sieve;
    sieve_len = new_len;
}

static bool is_prime(idx_t n) {
    if (n > sieve_len) {
        sieve_of_eratosthenes(n);
    }
    return is_prime_arr[n];
}

static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_cap = 0;

static void ensure_capacity(idx_t n) {
    if (n < memo_cap) return;
    
    idx_t new_cap = (memo_cap == 0) ? 100 : memo_cap * 2;
    while (new_cap <= n) new_cap *= 2;
    
    seq_t *new_memo = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    bool *new_computed = (bool *)malloc((new_cap + 1) * sizeof(bool));
    
    memset(new_memo, 0, (new_cap + 1) * sizeof(seq_t));
    memset(new_computed, 0, (new_cap + 1) * sizeof(bool));
    
    if (memo != NULL) {
        memcpy(new_memo, memo, (memo_cap + 1) * sizeof(seq_t));
        memcpy(new_computed, computed, (memo_cap + 1) * sizeof(bool));
        free(memo);
        free(computed);
    }
    
    memo = new_memo;
    computed = new_computed;
    memo_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    ensure_capacity(n);
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result;
    
    if (n == 1) {
        result = 0;
    } else if (n == 2) {
        result = -2;
    } else {
        if (is_prime(n)) {
            int sign_n = ((n + 1) % 2 == 0) ? 1 : -1;
            result = sign_n * (seq_t)n;
        } else {
            result = 0;
        }
        
        for (idx_t k = 1; k < n; k++) {
            if (is_prime(n - k)) {
                int sign_k = ((n - k + 1) % 2 == 0) ? 1 : -1;
                result += sign_k * a(k);
            }
        }
    }
    
    memo[n] = result;
    computed[n] = true;
    return result;
}
