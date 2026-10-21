/*
 * A232343  Expansion of q^(-5/3) * c(q^2)^3 / (9 * c(q)) in powers of q where
 *          c() is a cubic AGM theta function.
 * https://oeis.org/A232343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 37, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = 1/6 * b(3*n + 5)
 *  where b(m) is multiplicative with:
 *    b(1) = 1
 *    if m is divisible by 3 then b(m) = 0
 *    for prime factor 2^e : contribution = 2 - 2^e
 *    for prime factor p>3 of power e : contribution = 1 + p + p^2 + ... + p^e
 *
 *  This implementation pre‑computes a small sieve of primes once and
 *  uses it to factor 3*n+5.  For n up to the order of 10^4 the
 *  sieve limit of 100 000 is more than sufficient.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------- Prime sieve (once per program run) --------- */
#define PRIME_SIEVE_LIMIT 100000

static uint32_t *primes = NULL;   /* array of primes */
static size_t prime_cnt = 0;      /* number of primes */

/* Initialise the prime table (sieve of Eratosthenes). */
static void init_prime_table(void)
{
    if (primes != NULL)
        return;   /* already initialised */

    /* Allocate temporary boolean array for the sieve */
    bool *is_composite = (bool *)calloc((size_t)PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (is_composite == NULL)
        return;   /* allocation failed – we will refuse to compute */

    /* Sieving */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            ++prime_cnt;

    /* Allocate prime array */
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (primes == NULL) {
        free(is_composite);
        return;
    }

    /* Store primes */
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            primes[idx++] = i;

    free(is_composite);
}

/* --------- Multiplicative function b(m) --------- */
static seq_t compute_b(idx_t m)
{
    /* b(m)=0 if m has a factor 3 */
    if (m % 3 == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;   /* multiplicative identity */
    idx_t temp = m;

    /* Factorisation using the pre‑computed primes */
    for (size_t pi = 0; pi < prime_cnt && (uint64_t)primes[pi] * primes[pi] <= temp; ++pi) {
        uint32_t p = primes[pi];
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            if (p == 2) {
                /* contribution = 2 - 2^exp */
                seq_t pow2 = (seq_t)1;
                for (uint32_t k = 0; k < exp; ++k)
                    pow2 *= (seq_t)2;
                result *= ((seq_t)2 - pow2);
            } else { /* p > 3 */
                /* contribution = 1 + p + ... + p^exp */
                seq_t sum = (seq_t)1;
                seq_t term = (seq_t)1;
                for (uint32_t k = 1; k <= exp; ++k) {
                    term *= (seq_t)p;
                    sum += term;
                }
                result *= sum;
            }
        }
    }

    /* Remaining prime factor > sqrt(m) (must have exponent 1) */
    if (temp > 1) {
        uint32_t p = (uint32_t)temp;
        if (p == 2) {
            seq_t pow2 = (seq_t)1;
            /* exponent is 1, so 2^1 = 2 */
            result *= ((seq_t)2 - pow2 * (seq_t)2);
        } else { /* p > 3 and not divisible by 3 (already checked) */
            seq_t sum = (seq_t)1 + (seq_t)p; /* 1 + p^1 */
            result *= sum;
        }
    }

    return result;
}

/* --------- Public sequence function --------- */
seq_t a(idx_t n)
{
    /* Ensure prime table is ready */
    init_prime_table();

    /* Compute m = 3*n + 5 (in idx_t for factorisation) */
    idx_t m = (idx_t)3 * n + (idx_t)5;

    /* b(m) * 1/6 */
    seq_t val = compute_b(m);
    val /= (seq_t)6;
    return val;
}
