/*
 * A096532  Number of composite numbers not greater than the n-th composite
 *          number that do not divide any number not greater than the n-th
 *          composite number.
 * https://oeis.org/A096532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - ⌊N/2⌋ + π(⌊N/2⌋) + 1, where N is the n‑th composite
 *            number. The algorithm uses a dynamic sieve to get π(x) and to
 *            find N via binary search
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---- global data ---- */
static uint8_t *is_prime  = NULL;      /* 1 if prime, 0 otherwise (index 0…limit)   */
static uint32_t *prime_pi = NULL;      /* π(i)  for 0…limit                          */
static idx_t    sieve_limit = 0;       /* current maximum sieved index               */

/* ---- free old tables ---- */
static void free_sieve(void) {
    free(is_prime);
    free(prime_pi);
    is_prime  = NULL;
    prime_pi = NULL;
    sieve_limit = 0;
}

/* ---- rebuild a sieve up to 'limit' ---- */
static void rebuild_sieve(idx_t limit) {
    free_sieve();
    is_prime  = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    prime_pi = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!is_prime || !prime_pi) exit(1);

    /* initialise primes as true, then apply classic sieve */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    /* build prefix prime counts */
    uint32_t pc = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++pc;
        prime_pi[i] = pc;
    }

    sieve_limit = limit;
}

/* ---- composite count up to x (inclusive) ---- */
static inline idx_t composite_count(idx_t x) {
    if (x < 2) return 0;
    return x - prime_pi[x] - 1ULL;
}

/* ---- make sure the sieve covers at least 'n' composites ---- */
static void ensure_limit_for_n(idx_t n) {
    if (sieve_limit == 0) rebuild_sieve(20); /* initial small table */
    if (composite_count(sieve_limit) >= n) return;

    idx_t new_limit = sieve_limit;
    if (new_limit == 0) new_limit = 20;
    while (composite_count(new_limit) < n) {
        new_limit *= 2;
    }
    rebuild_sieve(new_limit);
}

/* ---- main sequence function ---- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for 0, but harmless */
    ensure_limit_for_n(n);

    /* binary search for the n‑th composite N */
    idx_t low = 2, high = sieve_limit;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (composite_count(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t N = low;
    idx_t L = N / 2;                 /* floor(N/2) */
    uint32_t piL = prime_pi[L];

    seq_t res = (seq_t)n - (seq_t)L + (seq_t)piL + (seq_t)1;
    return res;
}
