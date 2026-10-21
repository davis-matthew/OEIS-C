/*
 * A059479  Number of 3 X 3 matrices with elements from {0,...,n-1} such that
 *          the middle element of each of the eight lines of three (rows,
 *          columns and diagonals) is the square (mod n) of the difference of
 *          the end elements.
 * https://oeis.org/A059479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A008833(n) * n^2 where A008833(n) is the largest square
 *            dividing n. The sequence is multiplicative: a(p^e) =
 *            p^(3e-(e%2)). This implementation factors n with a precomputed
 *            prime table (sieve up to 1 000 000) and multiplies the largest
 *            square divisor by n². It works for any seq_t type (fixe...
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 1680, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------ prime table ------------------------------------------------- */

static bool primes_initialized = false;
static uint32_t *primes = NULL;
static size_t primes_count = 0;

static void init_primes(void)
{
    /* Large sieve: enough to factor all n up to ~10^12. */
    const uint32_t LIMIT = 1000000U;          /* 1 000 000 */
    uint8_t *sieve = (uint8_t *)malloc((size_t)LIMIT + 1);
    if (!sieve) return;
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) sieve[j] = 0;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) primes[idx++] = i;

    primes_count = cnt;
    free(sieve);
    primes_initialized = true;
}

/* ------------ largest square divisor -------------------------------------- */

static seq_t largest_square_divisor(uint64_t n)
{
    if (!primes_initialized) init_primes();
    seq_t sq = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;

        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            uint64_t use = exp / 2;          /* number of p² factors */
            for (uint64_t k = 0; k < use; ++k) sq *= (seq_t)p * (seq_t)p;
        }
    }
    /* Remaining prime factor (exponent 1) contributes nothing. */
    return sq;
}

/* -------------------- main sequence function ------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1: not used for n=0 */

    seq_t sn = (seq_t)n;                  /* convert to seq_t */
    seq_t sq = largest_square_divisor(n);
    return sn * sn * sq;                  /* n^2 * largest square divisor */
}
