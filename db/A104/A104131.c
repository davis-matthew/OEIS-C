/*
 * A104131  a(n) = pip(n)^pip(n) where pip(n) is the n-th prime-indexed prime
 *          (see A006450).
 * https://oeis.org/A104131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pip(n)^pip(n) where pip(n) = p_{p_n} (prime indexed
 *            prime)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq_local.h"

/* Forward declaration for the arbitrary-precision exponent helper */
static seq_t pow_seq_local(seq_t base, idx_t exp);

/* Prime sieve data */
static bool primes_initialized = false;
static idx_t *primes_table = NULL;   /* 1‑based: primes_table[i] = i‑th prime */
static size_t primes_count   = 0;    /* number of primes stored */

/* Initialise the prime table once, up to a limit that guarantees
   at least the 104,729‑th prime (needed for n up to 10,000). */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* 1,600,000 is safely above the 104,729‑th prime (≈1,298,989). */
    const idx_t LIMIT = 1600000u;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return;  /* allocation failure – give up silently */

    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = true;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes to size the table */
    size_t count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++count;

    primes_table = (idx_t *)malloc((count + 1) * sizeof(idx_t));
    if (!primes_table) {
        free(is_prime);
        return;  /* allocation failure – give up */
    }

    primes_count = count;
    idx_t idx = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i])
            primes_table[idx++] = i;
    }

    free(is_prime);
    primes_initialized = true;
}

/* The requested OEIS sequence function */
seq_t a(idx_t n)
{
    /* OEIS offset 1: return 0 for out‑of‑range inputs */
    if (n == (idx_t)0)
        return (seq_t)0;

    init_primes();

    /* Safety: if the requested index exceeds our pre‑computed range */
    if (n > (idx_t)primes_count)
        return (seq_t)0;

    /* pip(n) = prime at index p_n */
    idx_t nth_prime_value = primes_table[n];           /* p_n */
    if (nth_prime_value > (idx_t)primes_count)
        return (seq_t)0; /* should not happen for n <= 10000 */

    idx_t pip_value = primes_table[nth_prime_value];   /* p_{p_n} */

    /* Return pip_value ^ pip_value */
    return pow_seq_local((seq_t)pip_value, (idx_t)pip_value);
}
