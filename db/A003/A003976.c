/*
 * A003976  Möbius transform of A003963 (with alternate 0's omitted).
 * https://oeis.org/A003976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = A003963(2n-1), where A003963 is multiplicative with
 *            a(p^e) = (q-1)*q^(e-1) for q = pi(p)
 * Range:     u8 n <= 810, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define SIEVE_SIZE 20001

static bool sieve_init_done = false;
static uint8_t sieve[SIEVE_SIZE];

static void init_sieve() {
    if (sieve_init_done) return;
    
    memset(sieve, 0, sizeof(sieve));
    sieve[0] = sieve[1] = 1;
    
    for (idx_t i = 2; i * i < SIEVE_SIZE; i++) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j < SIEVE_SIZE; j += i) {
                sieve[j] = 1;
            }
        }
    }
    
    sieve_init_done = true;
}

static idx_t prime_count(idx_t p) {
    init_sieve();
    
    idx_t count = 0;
    for (idx_t i = 2; i <= p && i < SIEVE_SIZE; i++) {
        if (!sieve[i]) count++;
    }
    return count;
}

static seq_t ipow(seq_t b, idx_t e) {
    seq_t result = 1;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

static seq_t a003963(idx_t m) {
    if (m % 2 == 0) return 0;
    if (m == 1) return 1;
    
    init_sieve();
    
    seq_t result = 1;
    idx_t remaining = m;
    
    for (idx_t p = 3; p * p <= remaining && p < SIEVE_SIZE; p += 2) {
        if (!sieve[p]) {
            idx_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                e++;
            }
            if (e > 0) {
                idx_t q = prime_count(p);
                seq_t val = (seq_t)(q - 1) * ipow((seq_t)q, e - 1);
                result *= val;
            }
        }
    }
    
    if (remaining > 1) {
        idx_t q = prime_count(remaining);
        result *= (seq_t)(q - 1);
    }
    
    return result;
}

seq_t a(idx_t n) {
    return a003963(2 * n - 1);
}
