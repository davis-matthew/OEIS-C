/*
 * A177082  a(n) = 2*n*A071148(n).
 * https://oeis.org/A177082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n * sum_{k=2}^{n+1} prime(k)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 842, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of all elements of the n×n matrix M(i,j)=prime(i+1)+prime(j+1) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime sieve – pre‑compute all primes up to a fixed upper bound.    */
/* ------------------------------------------------------------------ */
#define MAX_PRIME_LIMIT   200000   /* sufficient for n up to ≈10000      */

static int64_t primes_arr[18000];   /* > number of primes ≤200000 (≈17984) */
static size_t primes_len = 0;
static int  primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized) return;

    static unsigned char is_composite[MAX_PRIME_LIMIT + 1];
    for (idx_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes_arr[primes_len++] = (int64_t)i;
            if (i * i <= MAX_PRIME_LIMIT) {
                for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    primes_initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Sequence a(n)                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard for n < 1 */

    /* Ensure primes are available */
    init_primes();

    /* Sum primes 2..n+1  → indices 1..n in primes_arr             */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += (seq_t)primes_arr[i];

    /* a(n) = 2 * n * sum */
    seq_t result = sum * ((seq_t)2 * (seq_t)n);
    return result;
}
