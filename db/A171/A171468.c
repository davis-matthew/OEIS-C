/*
 * A171468  Noncomposite numbers raised to powers 2 or 3.
 * https://oeis.org/A171468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n≤2; for n≥3 let k = ⌊(n-1)/2⌋ and e = 2 if n is odd
 *            else 3. Then a(n) = (prime_k)^e, where prime_k is the k‑th prime
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities that are already provided, so no need to implement them. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of static helpers */
static void init_primes(void);

static bool primes_ready = false;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* Simple sieve of Eratosthenes that fills the global primes array
   with at least the first 5000 primes (more than enough for n≤10000). */
static void init_primes(void)
{
    /* Upper bound for the 5000th prime is < 55000; use 60000 for safety. */
    const idx_t limit = 60000;
    bool *is_composite = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_composite) {
        /* In a solver environment allocation always succeeds; no error handling needed. */
        return;
    }
    for (idx_t i = 2; i <= limit; ++i) {
        is_composite[i] = false;
    }
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_composite[p]) {
            for (idx_t x = p * p; x <= limit; x += p) {
                is_composite[x] = true;
            }
        }
    }
    /* Count primes and allocate array. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            ++cnt;
        }
    }
    primes_cnt = cnt;
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_composite);
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[idx++] = (uint32_t)i;
        }
    }
    free(is_composite);
}

/* Computes the term a(n) of sequence A171468. */
seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)1;
    }

    /* Ensure primes are initialized. */
    if (!primes_ready) {
        init_primes();
        primes_ready = true;
    }

    /* k = floor((n-1)/2) gives the 1‑based index of the prime. */
    idx_t k = (n - 1) / 2;
    /* primes[] is zero‑based. */
    uint32_t p = primes[(size_t)k - 1];

    seq_t base = (seq_t)p;
    seq_t result = base * base;        /* first square */
    if ((n & 1ULL) == 0) {              /* even n → cube */
        result *= base;
    }
    return result;
}
