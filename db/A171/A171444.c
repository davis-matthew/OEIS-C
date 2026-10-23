/*
 * A171444  Sum of three consecutive reversed primes.
 * https://oeis.org/A171444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = r(p(n)) + r(p(n+1)) + r(p(n+2)) where p(k) is the k‑th
 *            prime and r(x) reverses the decimal digits of x. This
 *            implementation dynamically generates the required primes using a
 *            simple sieve and computes the digit reversal manually
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_upper_bound_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_upper_bound_primes.h"

static uint64_t *primes = NULL;
static size_t prime_count = 0;
static uint8_t *composite = NULL;
static size_t composite_cap = 0;

static void ensure_primes(size_t required) {
    if (prime_count >= required) return;

    size_t bound = estimate_upper_bound_primes(required);
    if (bound <= composite_cap && bound <= prime_count) return; /* should not happen */

    /* allocate or re‑allocate composite array */
    free(composite);
    composite = (uint8_t*)calloc(bound + 1, sizeof(uint8_t));
    composite_cap = bound + 1;

    /* simple sieve of Eratosthenes */
    size_t i, j;
    size_t sqrtb = (size_t)sqrt((double)bound);
    for (i = 2; i <= sqrtb; ++i) {
        if (!composite[i]) {
            for (j = i * i; j <= bound; j += i) composite[j] = 1;
        }
    }

    /* count primes */
    size_t primes_needed = 0;
    for (i = 2; i <= bound; ++i) if (!composite[i]) ++primes_needed;

    /* allocate and fill primes array */
    uint64_t *new_primes = (uint64_t*)malloc(primes_needed * sizeof(uint64_t));
    size_t idx = 0;
    for (i = 2; i <= bound; ++i) if (!composite[i]) new_primes[idx++] = i;

    free(primes);
    primes = new_primes;
    prime_count = primes_needed;
}

static seq_t rev_uint64(uint64_t x) {
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n) {
    /* the sequence is defined for n >= 1; behaviour for smaller indices is irrelevant */
    if (n == 0) return 0;

    /* we need the (n+2)-th prime */
    ensure_primes((size_t)n + 2);

    seq_t sum = rev_uint64(primes[(size_t)n - 1])   /* p(n)   */
             + rev_uint64(primes[(size_t)n])       /* p(n+1) */
             + rev_uint64(primes[(size_t)n + 1]);  /* p(n+2) */

    return sum;
}
