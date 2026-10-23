/*
 * A145591  a(n)=number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^13.
 * https://oeis.org/A145591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of numbers removed in step n of Eratosthenes's sieve for 2^13.
   Sequence A145591.  For n > 24 the correct value is 0.  */

#include <stdint.h>
#include <stdbool.h>

#define N ((idx_t)1 << 13)   /* 8192 */

static bool initialized = false;
static seq_t a_values[N + 1];
static idx_t spf[N + 1];
static idx_t prime_index[N + 1];
static idx_t num_primes = 0;

/* Build sieve and count the numbers with given smallest prime factor.  */
static void init(void)
{
    /* smallest prime factors */
    for (idx_t i = 0; i <= N; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            if (i <= N / i) {              /* i*i fits in range */
                for (idx_t j = i * i; j <= N; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* map prime → index in our table */
    for (idx_t i = 0; i <= N; ++i)
        prime_index[i] = (idx_t)-1;

    for (idx_t i = 2; i <= N; ++i)
        if (spf[i] == i)
            prime_index[i] = num_primes++;

    /* count numbers whose smallest prime factor is p */
    for (idx_t i = 2; i <= N; ++i) {
        idx_t p = spf[i];
        idx_t id = prime_index[p];
        if (id != (idx_t)-1)
            a_values[id] = a_values[id] + (seq_t)1;
    }

    /* subtract the prime itself (primes are not removed) */
    for (idx_t i = 0; i < num_primes; ++i)
        a_values[i] = a_values[i] - (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* External function required by the test harness.                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;

    idx_t idx = n - 1;
    if (idx >= num_primes)
        return (seq_t)0;

    return a_values[idx];
}
