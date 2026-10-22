/*
 * A000720  pi(n), the number of primes <= n. Sometimes called PrimePi(n) to
 *          distinguish it from the number 3.14159...
 * https://oeis.org/A000720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   π(n) computed using Sieve of Eratosthenes
 * Range:     u8 n <= 1618, u16 n <= 9674, u32 n <= 9674, u64 n <= 9674,
 *            u128 n <= 9674, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static char *sieve = NULL;
static idx_t sieve_len = 0;

static void ensure_sieve(idx_t n) {
    if (sieve && sieve_len > n) return;
    
    if (sieve) free(sieve);
    sieve_len = n + 1;
    sieve = (char *)malloc(sieve_len);
    if (!sieve) {
        sieve_len = 0;
        return;
    }
    
    memset(sieve, 1, sieve_len);
    if (sieve_len > 0) sieve[0] = 0;
    if (sieve_len > 1) sieve[1] = 0;
    
    for (idx_t i = 2; i * i <= n; i++) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                sieve[j] = 0;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    
    ensure_sieve(n);
    
    if (!sieve) return 0;
    
    seq_t count = 0;
    for (idx_t i = 2; i <= n; i++) {
        if (sieve[i]) count++;
    }
    return count;
}
