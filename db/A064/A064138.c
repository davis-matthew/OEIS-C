/*
 * A064138  Sum of non-unitary divisors of n!.
 * https://oeis.org/A064138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n!) - usigma(n!) where sigma is the sum of all
 *            divisors of n! and usigma is the sum of the unitary (pair‑wise
 *            coprime) divisors of n!. Computation is done by prime
 *            factorising n! and applying the multiplicative formulas for
 *            sigma and for the unitary sum
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- small power function for seq_t ------------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* ---------- global sieve data ------------------------- */
#define MAX_PRIMES 20000          /* enough for n up to many thousands */

static idx_t primes[MAX_PRIMES];
static size_t prime_count = 0;
static idx_t sieve_limit = 0;
static bool *is_prime = NULL;

/* ---------- grow the sieve to at least 'limit' --------- */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                     /* already fitted */

    /* free old mask if any */
    if (is_prime)
        free(is_prime);

    sieve_limit = limit;
    is_prime = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
    if (!is_prime)
        return;                     /* allocation failed – we'll return 0 soon */

    /* initialize */
    for (idx_t i = 0; i <= sieve_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= sieve_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= sieve_limit; mult += p)
                is_prime[mult] = false;
        }
    }

    /* collect primes up to sieve_limit */
    prime_count = 0;
    for (idx_t i = 2; i <= sieve_limit && prime_count < MAX_PRIMES; ++i)
        if (is_prime[i])
            primes[prime_count++] = i;
}

/* ---------- main sequence function -------------------- */
seq_t a(idx_t n)
{
    if (n < 1)          /* sequence is defined for n >= 1 */
        return 0;

    ensure_primes(n);
    if (!is_prime)      /* memory allocation failure */
        return 0;

    seq_t sigma_total = 1;      /* product of sigma(p^e) for each p */
    seq_t unitary_total = 1;    /* product of (1 + p^e) */

    for (size_t idx = 0; idx < prime_count && primes[idx] <= n; ++idx) {
        idx_t p = primes[idx];

        /* exponent e of prime p in n! (Legendre's formula) */
        idx_t e = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            e += t;
        }
        if (e == 0)
            continue;

        /* compute p^e as seq_t */
        seq_t p_pow_e = pow_seq((seq_t)p, e);

        /* sigma term: (p^(e+1)-1)/(p-1) */
        seq_t numerator = p_pow_e * (seq_t)p - 1;
        seq_t denominator = (seq_t)p - 1;
        seq_t sigma_term = numerator / denominator;

        /* unitary term: 1 + p^e */
        seq_t unitary_term = p_pow_e + 1;

        sigma_total *= sigma_term;
        unitary_total *= unitary_term;
    }

    return sigma_total - unitary_total;
}
