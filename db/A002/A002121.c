/*
 * A002121  a(0) = 1, a(1) = 0, a(2) = -1; for n >= 3, a(n) = - a(n-2) + Sum_{
 *          primes p with 3 <= p <= n} a(n-p).
 * https://oeis.org/A002121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = -a(n-2) + Sum_{primes p with 3 <= p <= n} a(n-p), with
 *            a(0) = 1, a(1) = 0, a(2) = -1
 * Range:     i8 n <= 31, i16 n <= 66, i32 n <= 132, i64 n <= 265,
 *            i128 n <= 527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CACHE 10000

static seq_t cache[MAX_CACHE + 1];
static bool cache_valid[MAX_CACHE + 1];
static bool is_prime[MAX_CACHE + 1];
static int sieve_done = 0;

static void sieve_primes(void) {
    if (sieve_done) return;
    sieve_done = 1;
    
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;
    
    for (idx_t i = 2; i * i <= MAX_CACHE; i++) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_CACHE; j += i) {
                is_prime[j] = 0;
            }
        }
    }
}

static bool is_prime_check(idx_t n) {
    if (n <= MAX_CACHE) {
        return is_prime[n];
    }
    
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    if (n == 3) return 1;
    if (n % 3 == 0) return 0;
    
    for (idx_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n) {
    sieve_primes();
    
    if (n <= MAX_CACHE && cache_valid[n]) {
        return cache[n];
    }
    
    seq_t result;
    
    if (n == 0) {
        result = 1;
    } else if (n == 1) {
        result = 0;
    } else if (n == 2) {
        result = -1;
    } else {
        result = -a(n - 2);
        for (idx_t p = 3; p <= n; p++) {
            if (is_prime_check(p)) {
                result += a(n - p);
            }
        }
    }
    
    if (n <= MAX_CACHE) {
        cache[n] = result;
        cache_valid[n] = 1;
    }
    
    return result;
}
