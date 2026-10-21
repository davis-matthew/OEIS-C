/*
 * A076080  a(n) = A076079(n)/n.
 * https://oeis.org/A076080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n) - 1) / n)
 * Range:     u8 n <= 54, u16 n <= 7342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A076080.  It computes primes by a simple sieve up to a
   fixed limit that is sufficient for n up to about 10 000.  The sieve
   data is built once on the first call. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Parameters chosen to cover n ≈ 10 000 (prime_10000 = 104 729).      */
/* ------------------------------------------------------------------ */
#define SIEVE_LIMIT      200000      /* 200 000 > prime(20000) */
#define MAX_PRIMES       20000       /* enough primes under SIEVE_LIMIT */

/* static data */
static bool   sieve_composite[SIEVE_LIMIT + 1];
static uint64_t primes[MAX_PRIMES];
static size_t  prime_count = 0;
static bool    sieve_built  = false;

/* ------------------------------------------------------------------ */
/* Build the sieve once.                                               */
/* ------------------------------------------------------------------ */
static void build_sieve(void)
{
    size_t i, j;

    prime_count = 0;

    for (i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve_composite[i]) {
            if (prime_count < MAX_PRIMES)
                primes[prime_count++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve_composite[j] = true;
            }
        }
    }

    sieve_built = true;
}

/* ------------------------------------------------------------------ */
/* The sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined below offset */

    if (!sieve_built)
        build_sieve();

    /* Guard against out‑of‑range n: return 0 for robustness */
    if ((size_t)n > prime_count)
        return 0;

    seq_t p_n = (seq_t)primes[n - 1];
    seq_t res = (p_n - 1) / ((seq_t)n);
    return res;
}
