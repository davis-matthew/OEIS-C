/*
 * A214743  Least m>0 such that n-m divides prime(n)+prime(m).
 * https://oeis.org/A214743
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214743
   a(n) = least m>0 such that (n-m) divides prime(n)+prime(m)
   offset 2 (a(2) is the first term) */

#include <stdlib.h>
#include <stdint.h>

/* Prime storage */
static uint32_t *primes_arr = NULL;   /* primes_arr[0] = 2, primes_arr[1] = 3, ... */
static size_t   primes_alloc = 0;     /* allocated array size               */
static idx_t    primes_count = 0;     /* number of primes stored            */
static idx_t    sieve_limit = 0;      /* highest number sieved previously   */

/* Ensure that primes_arr contains at least n primes.
   The bound n*20+100 is safe for n up to several million. */
static void ensure_primes(idx_t n)
{
    if (n <= primes_count) return;   /* already have enough primes */

    /* compute sieve limit */
    idx_t limit = n * 20 + 100;
    if (limit <= sieve_limit) limit = sieve_limit + 1;

    /* simple sieve up to 'limit' */
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (comp == NULL) return;        /* out of memory – caller will fail safely */

    for (idx_t p = 2; p * p <= limit; ++p)
        if (!comp[p])
            for (idx_t q = p * p; q <= limit; q += p)
                comp[q] = 1;

    /* count primes in the range 2..limit */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++count;

    /* grow the primes array if needed */
    if (count > primes_alloc) {
        uint32_t *new_arr = (uint32_t *)realloc(primes_arr, count * sizeof(uint32_t));
        if (new_arr == NULL) { free(comp); return; }
        primes_arr = new_arr;
        primes_alloc = count;
    }

    /* fill the array with the primes */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i]) primes_arr[idx++] = (uint32_t)i;

    primes_count = count;
    sieve_limit  = limit;
    free(comp);
}

/* Compute a(n) as defined in A214743 */
seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined below offset; return 0 */

    ensure_primes(n);                      /* guarantee we have prime(n) */
    seq_t prime_n = (seq_t)primes_arr[n - 1];/* prime(n) */

    for (idx_t m = 1; m < n; ++m)
    {
        seq_t prime_m = (seq_t)primes_arr[m - 1];
        seq_t diff    = (seq_t)(n - m);      /* diff = n-m (>0) */
        seq_t sum     = prime_n + prime_m;   /* prime(n)+prime(m) */
        if (sum % diff == 0)                 /* divisible? */
            return (seq_t)m;                 /* return the smallest such m */
    }

    /* Theoretically unreachable – return n-1 as a fallback. */
    return (seq_t)(n - 1);
}
