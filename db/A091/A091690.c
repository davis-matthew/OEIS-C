/*
 * A091690  Smallest integer m such that the n-th prime A000040(n) divides
 *          partition number A000041(m).
 * https://oeis.org/A091690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A091690
   a(n) = smallest integer m such that the n-th prime divides the partition number p(m) */
/* The function uses only integer arithmetic and a simple sieve for primes up to the 10000th prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N_PRIMES 200000   /* enough to contain the 10000th prime (≈104729) */
#define MAX_N          10000  /* upper limit for which the sequence is requested */

/*---------------------------------------------------------------------------*/
/* Smallest n-th prime via linear sieve                                       */
/*---------------------------------------------------------------------------*/
static bool sieve_done = false;
static uint32_t primes[MAX_N_PRIMES];
static int prime_count = 0;

/* build the prime table once */
static void init_prime_sieve(void)
{
    if (sieve_done) return;
    bool *is_comp = (bool *)calloc(MAX_N_PRIMES + 1, sizeof(bool));
    if (!is_comp) return; /* out of memory – will lead to failure below */

    for (int i = 2; i <= MAX_N_PRIMES; ++i) {
        if (!is_comp[i]) {
            primes[prime_count++] = (uint32_t)i;
            if ((int64_t)i * i <= MAX_N_PRIMES) {
                for (int j = i * i; j <= MAX_N_PRIMES; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    sieve_done = true;
}

/* return the n-th prime (1‑based) */
static uint32_t nth_prime(idx_t n)
{
    init_prime_sieve();
    if (n == 0 || n > prime_count) return 0; /* error */
    return primes[n - 1];
}

/*---------------------------------------------------------------------------*/
/* Main sequence function                                                      */
/*---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of range */
    uint32_t prime = nth_prime(n);
    if (prime == 0) return (seq_t)0; /* out of range or error */

    uint64_t mod = (uint64_t)prime;

    /* dynamic array for partition numbers modulo prime */
    size_t cap = 128;
    uint64_t *p = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!p) return (seq_t)0; /* allocation failure */

    p[0] = 1 % mod;   /* p(0) = 1 */

    size_t m;
    for (m = 1; ; ++m) {
        if (m >= cap) {
            cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(p, cap * sizeof(uint64_t));
            if (!tmp) { free(p); return (seq_t)0; }
            p = tmp;
        }

        uint64_t sum = 0;
        for (int k = 1; ; ++k) {
            /* generalized pentagonal numbers */
            uint64_t pent1 = (uint64_t)k * (3ULL * (uint64_t)k - 1ULL) / 2ULL;
            if (pent1 > m) break;

            uint64_t term = p[m - pent1];
            if ((k & 1) == 0) {                  /* sign = -1 for even k */
                if (sum >= term)
                    sum -= term;
                else
                    sum = sum + mod - term;
            } else {                              /* sign = +1 for odd k */
                sum += term;
                if (sum >= mod) sum -= mod;
            }

            uint64_t pent2 = (uint64_t)k * (3ULL * (uint64_t)k + 1ULL) / 2ULL;
            if (pent2 <= m) {
                term = p[m - pent2];
                if ((k & 1) == 0) {
                    if (sum >= term)
                        sum -= term;
                    else
                        sum = sum + mod - term;
                } else {
                    sum += term;
                    if (sum >= mod) sum -= mod;
                }
            }
        }

        p[m] = sum;
        if (p[m] == 0) {
            free(p);
            return (seq_t)m;
        }
    }
}
