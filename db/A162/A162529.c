/*
 * A162529  Numbers k whose largest divisor <= sqrt(k) equals 9.
 * https://oeis.org/A162529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*prime(n-4) for n > 13; the first 13 terms are listed
 *            explicitly
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint32_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap = 0;
static uint32_t next_candidate = 3; /* next odd number to test for primality */

/* Ensure the prime array has enough capacity */
static void ensure_prime_capacity(void) {
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    } else if (primes_count >= primes_cap) {
        primes_cap <<= 1;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }
}

/* Initialize the prime list with 2 if it is empty */
static void init_primes(void) {
    if (primes_count == 0) {
        ensure_prime_capacity();
        primes[primes_count++] = 2U;
        next_candidate = 3U;
    }
}

/* Extend the prime list until it contains at least target_index+1 primes (0‑based). */
static void extend_primes_to_index(size_t target_index) {
    init_primes();
    while (primes_count <= target_index) {
        bool isprime = true;
        for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * primes[i] <= next_candidate; ++i) {
            if (next_candidate % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            ensure_prime_capacity();
            primes[primes_count++] = next_candidate;
        }
        next_candidate += 2; /* skip even numbers */
    }
}

/* The required OEIS sequence */
seq_t a(idx_t n) {
    /* first 13 terms are hard‑coded */
    static const seq_t base[13] = {
        (seq_t)81,  (seq_t)90,  (seq_t)99,  (seq_t)108, (seq_t)117,
        (seq_t)126, (seq_t)135, (seq_t)153, (seq_t)162, (seq_t)171,
        (seq_t)189, (seq_t)207, (seq_t)243
    };
    if (n <= 13) {
        return base[n - 1];
    }

    /* For n > 13: a(n) = 9 * prime(n-4)  (prime(1)=2) */
    size_t idx = (size_t)(n - 5); /* 0‑based index into primes array */
    extend_primes_to_index(idx);
    return (seq_t)9 * (seq_t)primes[idx];
}
