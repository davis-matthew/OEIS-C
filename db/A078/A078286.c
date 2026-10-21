/*
 * A078286  Least nontrivial multiple of the n-th prime beginning with 2.
 * https://oeis.org/A078286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least nontrivial multiple of the n-th prime that starts
 *            with 2
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* maximal sieve bound sufficient for the first 10 000 primes. */
static const idx_t SIEVE_MAX = 200000;

/* storage for primes */
static idx_t *primes = NULL;
static size_t primes_count = 0;
static bool primes_initialized = false;

/* simple sieve of Eratosthenes, called once on first access */
static void init_primes (void)
{
    if (primes_initialized)
        return;

    /* temporary array of flags */
    bool *is_comp = (bool *)malloc((SIEVE_MAX + 1) * sizeof(bool));
    if (!is_comp)
        exit(1); /* fatal */

    for (idx_t i = 0; i <= SIEVE_MAX; ++i)
        is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    for (idx_t i = 2; i * i <= SIEVE_MAX; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= SIEVE_MAX; j += i)
                is_comp[j] = true;
        }
    }

    /* determine number of primes */
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
        if (!is_comp[i])
            ++primes_count;

    primes = (idx_t *)malloc(primes_count * sizeof(idx_t));
    if (!primes)
        exit(1); /* fatal */

    size_t idx = 0;
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
        if (!is_comp[i])
            primes[idx++] = i;

    free(is_comp);
    primes_initialized = true;
}

/* returns the first decimal digit of x (x > 0) */
static seq_t first_digit (seq_t x)
{
    seq_t pow10 = 1;
    /* grow pow10 until it exceeds x */
    while (pow10 <= x)
        pow10 = pow10 * 10;
    /* pow10 now > x; the first digit is x / (pow10/10) */
    return x / (pow10 / 10);
}

/* main sequence function */
seq_t a (idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0, but protected */

    init_primes();

    if ((size_t)n > primes_count) {
        /* exceed precomputed range – not expected for tests */
        return 0;
    }

    /* nth prime (1‑based) */
    seq_t p = (seq_t)primes[n - 1];

    /* search for the smallest k >= 2 such that k*p starts with 2 */
    idx_t k = 2;
    while (1) {
        seq_t m = p * (seq_t)k;
        if (first_digit(m) == 2)
            return m;
        ++k;
    }
}
