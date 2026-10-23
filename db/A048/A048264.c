/*
 * A048264  Numbers that aren't the sum of distinct primes of the form 6k+5.
 * https://oeis.org/A048264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the nth positive integer not representable as sum of
 *            distinct primes ≡5 (mod 6)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *nonrep = NULL;
static size_t nonrep_len = 0;

/* Build the list of numbers that are not representable as a sum of distinct
   primes ≡5 (mod 6).  This is done once on first call. */
static void build_nonrep(void)
{
    const size_t LIMIT = 200;          /* > 161, the bound from Makowski's theorem */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;            /* allocation failure – give up */

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Collect primes ≡5 (mod 6) */
    size_t primes[128];
    size_t prime_cnt = 0;
    for (size_t p = 5; p <= LIMIT; p += 6) {
        if (is_prime[p]) {
            primes[prime_cnt++] = p;
        }
    }

    /* DP for subset sums of these primes */
    bool rep[201] = { false };   /* rep[s] == true if s is representable */
    rep[0] = true;
    for (size_t k = 0; k < prime_cnt; ++k) {
        size_t p = primes[k];
        for (size_t s = LIMIT; s >= p; --s) {
            if (rep[s - p]) rep[s] = true;
        }
    }

    /* Count how many numbers are not representable */
    size_t count = 0;
    for (size_t x = 1; x <= LIMIT; ++x)
        if (!rep[x]) ++count;

    if (count == 0) {
        free(is_prime);
        return;
    }

    nonrep = (seq_t *)malloc(count * sizeof(seq_t));
    if (!nonrep) {
        free(is_prime);
        return; /* allocation failure */
    }

    size_t idx = 0;
    for (size_t x = 1; x <= LIMIT; ++x) {
        if (!rep[x]) nonrep[idx++] = (seq_t)x;
    }
    nonrep_len = count;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (nonrep == NULL) build_nonrep();
    if (n == 0 || n > nonrep_len) return (seq_t)0; /* undefined for out of range */
    return nonrep[(size_t)(n - 1)];
}
