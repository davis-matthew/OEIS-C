/*
 * A086803  a(n) = Product_{i=2..n} (prime(n+1)-prime(i)).
 * https://oeis.org/A086803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{i=1..n} (prime(n+1) – prime(i)) (offset 1: a(1) =
 *            (prime(2)-prime(1)) = 1) The implementation lazily generates
 *            primes up to prime(n+1) and evaluates the product directly
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   upper_bound_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/upper_bound_prime.h"

/* Sequencing types are provided by the build system.  Do not define them here. */
/* idx_t   – unsigned 64‑bit index type
   seq_t   – unsigned integer type for sequence values */

/* ---- Prime storage --------------------------------- */
static idx_t *prime_list = NULL;   /* 0‑based array: prime_list[0] = 2 */
static idx_t  prime_cnt  = 0;      /* number of primes currently stored */

/* ---- Rebuild the prime list up to at least 'need' primes */
static void build_primes(idx_t need)
{
    idx_t limit = upper_bound_prime(need);
    size_t lim  = (size_t)limit + 1;

    unsigned char *sieve = (unsigned char*)malloc(lim);
    if (!sieve) return;                     /* allocation failed – drop */

    memset(sieve, 1, lim);                  /* assume prime */
    if (lim > 0) sieve[0] = 0;
    if (lim > 1) sieve[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;

    idx_t *new_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_list) { free(sieve); return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) new_list[pos++] = i;

    free(sieve);

    if (prime_list) free(prime_list);
    prime_list = new_list;
    prime_cnt  = (idx_t)cnt;
}

/* ---- Ensure we have at least 'need' primes in prime_list */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= need) return;
    build_primes(need);
}

/* ---- Sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* outside defined range */

    /* Need primes 1..n+1, i.e. at least n+1 primes. */
    ensure_primes(n + 1);

    idx_t pk   = prime_list[n];           /* prime(n+1) */
    seq_t prod = (seq_t)1;

    for (idx_t i = 0; i < n; ++i) {
        seq_t diff = (seq_t)pk - (seq_t)prime_list[i];
        prod = prod * diff;
    }
    return prod;
}
