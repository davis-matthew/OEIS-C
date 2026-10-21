/*
 * A082974  a(n) = (a(n-1) + p(n)) mod p(n+1).
 * https://oeis.org/A082974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(n) = (a(n-1) + p(n)) mod p(n+1) (n >= 2) where p(k)
 *            is the k‑th prime (p(1)=2)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- Prime infrastructure ----------------------------------- */
static uint32_t *prime_array   = NULL;   /* 0‑based: prime_array[0] = 2          */
static size_t   prime_count    = 0;      /* number of primes stored              */
static size_t   prime_capacity = 0;      /* allocated capacity of prime_array    */
static uint32_t next_candidate = 2;      /* next integer to test for primality  */

/* Ensure that prime_array contains at least (index+1) primes
   (i.e. prime_array[index] is defined). */
static void ensure_primes_upto_index(idx_t index)
{
    /* Allocate initial storage if needed */
    if (prime_array == NULL) {
        prime_capacity = 128;
        prime_array = (uint32_t *)malloc(prime_capacity * sizeof(uint32_t));
        if (!prime_array) exit(1);
    }

    while (prime_count <= (size_t)index) {
        /* Test next_candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_array[i];
            uint64_t pp = (uint64_t)p * (uint64_t)p;
            if (pp > next_candidate) break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow array if necessary */
            if (prime_count >= prime_capacity) {
                prime_capacity *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_array,
                                                     prime_capacity * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_array = tmp;
            }
            prime_array[prime_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* --- Sequence cache ------------------------------------------ */
static seq_t *term_cache = NULL;  /* 0‑based: term_cache[i] = a(i+1)       */
static size_t cache_size = 0;     /* number of terms stored in term_cache */

/* Extend the cache so that term_cache[n-1] is computed (n >= 1) */
static void extend_cache_to(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    /* Ensure enough primes are available */
    ensure_primes_upto_index(n);           /* need p(n) and p(n+1) */

    /* Reallocate cache to new size */
    seq_t *new_cache = (seq_t *)realloc(term_cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) exit(1);
    term_cache = new_cache;

    /* Compute sequence terms incrementally */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)2;                       /* a(1) = 2 */
        } else {
            seq_t prev  = term_cache[i - 2];      /* a(i-1) */
            seq_t add   = (seq_t)prime_array[i - 1];   /* p(i)   */
            seq_t modp  = (seq_t)prime_array[i];      /* p(i+1) */
            val = (prev + add) % modp;
        }
        term_cache[i - 1] = val;
    }

    cache_size = (size_t)n;
}

/* --- Public function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the sequence */
    extend_cache_to(n);
    return term_cache[n - 1];
}
