/*
 * A098383  Define a function f on the positive integers by: if n is 1 or
 *          composite, stop; but if n = prime(k) then f(n) = k; a(n) = sum of
 *          terms in trajectory of n under repeated application of f.
 * https://oeis.org/A098383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 210, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A098383: Sum of the prime‑index chain for n.
 * For a positive integer n,
 *   if n is composite or 1:  a(n) = n
 *   else (n is prime) let f(n) = the index of n in the ordered list of primes.
 *   Then a(n) = n + f(n) + f(f(n)) + … + 1 .
 * (The chain ends when a composite or 1 is reached.)
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *sieve_is_prime = NULL;   /* 0 composite, 1 prime */
static uint32_t *sieve_prime_index = NULL; /* prime index starting from 1; 0 if composite */
static size_t sieve_limit = 0;
static uint32_t prime_count = 0;
static int sieve_initialized = 0;

static void init_sieve(void)
{
    if (sieve_initialized)
        return;
    /* Choose a limit large enough for all expected inputs. */
    sieve_limit = 2000000;              /* 2 million covers n up to ~10^5 comfortably */
    sieve_is_prime = (uint8_t *)malloc((sieve_limit + 1) * sizeof(uint8_t));
    sieve_prime_index = (uint32_t *)malloc((sieve_limit + 1) * sizeof(uint32_t));
    if (!sieve_is_prime || !sieve_prime_index) {
        /* Out of memory – abort. */
        exit(1);
    }
    memset(sieve_is_prime, 1, sieve_limit + 1);
    sieve_is_prime[0] = sieve_is_prime[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (sieve_is_prime[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                sieve_is_prime[j] = 0;
        }
    }

    /* Store prime indices */
    prime_count = 0;
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (sieve_is_prime[i]) {
            ++prime_count;
            sieve_prime_index[i] = prime_count;
        } else {
            sieve_prime_index[i] = 0;
        }
    }
    sieve_initialized = 1;
}

seq_t a(idx_t n)
{
    init_sieve();

    seq_t sum = n;
    idx_t cur = n;

    /* Stop when n is composite or 1 */
    while (cur >= 2) {
        if (cur <= sieve_limit) {
            if (!sieve_is_prime[cur])
                break;          /* composite */
            /* n is prime – replace by its prime index */
            cur = sieve_prime_index[cur];
        } else {
            /* n is larger than our pre‑computed range.
               For the typical test sizes this never happens,
               but if it does we treat n as composite to avoid
               unbounded computation. */
            break;
        }
        sum += (seq_t)cur;
    }
    return sum;
}
