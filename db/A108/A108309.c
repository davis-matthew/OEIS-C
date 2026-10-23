/*
 * A108309  Consider the triangle of odd numbers where the n-th row has the
 *          next n odd numbers. The sequence is the number of primes in the
 *          n-th row.
 * https://oeis.org/A108309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes in the interval [n^2-n+1 , n^2+n-1]
 *            (i.e. primes among the n odd numbers forming the n‑th row of the
 *            triangle). Implementation builds a prime sieve up to the largest
 *            required bound and uses binary search on the list of primes
 * Range:     u8 n <= 1778, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lower_bound, upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/lower_bound.h"
#include "../utils/upper_bound.h"

/* ------------------------------------------------------------------ */
/* ----  static state of the prime sieve  -------------------------- */
static uint8_t *prime_bits = NULL;   /* bit array: 1 = prime, 0 = composite   */
static uint32_t sieve_limit = 0;     /* maximum number sieved                */
static uint32_t *prime_list = NULL;  /* list of primes up to sieve_limit      */
static size_t prime_len   = 0;        /* number of primes stored               */
static size_t prime_cap   = 0;        /* capacity of prime_list                */

/* ------------------------------------------------------------------ */
/* ----  helpers for the bit array  -------------------------------- */
static inline int is_prime_bit(uint32_t x)
{
    return (prime_bits[x >> 3] >> (x & 7)) & 1;
}
static inline void set_composite(uint32_t x)
{
    prime_bits[x >> 3] &= (uint8_t)~(1 << (x & 7));
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* ----  build (or rebuild) the sieve  ------------------------------ */
static void rebuild_sieve(uint32_t limit)
{
    /* free old buffers if they exist */
    if (prime_bits) free(prime_bits);
    if (prime_list) free(prime_list);

    sieve_limit = limit;

    /* allocate bit array: one bit per number up to 'limit' */
    size_t bytes = (limit >> 3) + 1;
    prime_bits = (uint8_t *)malloc(bytes);
    if (!prime_bits) return;                      /* allocation failed */
    for (size_t i = 0; i < bytes; ++i) prime_bits[i] = 0xFF;  /* assume prime */

    if (limit >= 0)      set_composite(0);
    if (limit >= 1)      set_composite(1);

    /* simple sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime_bit(i)) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                set_composite((uint32_t)j);
        }
    }

    /* build prime list, using dynamic growth */
    prime_cap = (size_t)(limit / 10) + 1;            /* initial estimate */
    if (prime_cap < 1) prime_cap = 1;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) { prime_len = 0; return; }

    prime_len = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (is_prime_bit(i)) {
            if (prime_len == prime_cap) {
                prime_cap <<= 1;                    /* double capacity */
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) { prime_len = 0; return; }
                prime_list = tmp;
            }
            prime_list[prime_len++] = i;
        }
    }
}

/* ------------------------------------------------------------------ */
/* ----  sequence function  ------------------------------------------ */
seq_t a(idx_t n)
{
    /* offset: a(1) = 0 */
    if (n == 1) return 0;

    /* compute bounds that may be needed for this n */
    uint64_t n64 = (uint64_t)n;
    uint32_t need_lim = (uint32_t)(n64 * n64 + n64);  /* covers up to n^2+n-1 */

    /* rebuild sieve if necessary */
    if (!prime_bits || need_lim > sieve_limit)
        rebuild_sieve(need_lim);

    /* interval lower and upper bounds */
    uint32_t low  = (uint32_t)(n64 * n64 - n64 + 1);   /* n^2 - n + 1 */
    uint32_t high = (uint32_t)(n64 * n64 + n64 - 1);   /* n^2 + n - 1 */

    /* binary search on the prime list */
    size_t lo = lower_bound(prime_list, prime_len, low);
    size_t hi = upper_bound(prime_list, prime_len, high);

    seq_t count = (seq_t)(hi - lo);
    return count;
}
