/*
 * A036271  10th differences of primes.
 * https://oeis.org/A036271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0}^{10} (-1)^{10-i} * C(10,i) * p_{n+i} where p_k is
 *            the k‑th prime (p_1 = 2)
 * Range:     i8 none, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of primes.  prime_cache[0] = 2, prime_cache[1] = 3, …   */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt  = 0;   /* number of primes currently stored          */
static size_t prime_cap  = 0;   /* allocated capacity                        */

/* Ensure that prime_cache contains at least 'need' primes (1‑based).
   The array is 0‑based: prime_cache[i] == i+1-th prime.  */
static void ensure_prime_count(size_t need)
{
    if (need <= prime_cnt)
        return;

    /* Grow the storage.  */
    size_t new_cap = prime_cap;
    if (new_cap == 0)
        new_cap = need;
    while (new_cap < need)
        new_cap <<= 1;

    uint64_t *new_buf = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!new_buf)
        abort();          /* allocation failure */
    prime_cache = new_buf;
    prime_cap   = new_cap;

    /* Generate primes until we have 'need' of them.  */
    uint64_t candidate;
    if (prime_cnt == 0)
        candidate = 2;   /* first prime */
    else
    {
        candidate = prime_cache[prime_cnt - 1] + 1;
        if (candidate % 2 == 0)
            candidate++;        /* skip even numbers after 2 */
    }

    while (prime_cnt < need)
    {
        if (is_prime((idx_t)candidate))
            prime_cache[prime_cnt++] = candidate;
        candidate += (candidate == 2 ? 1 : 2);  /* advance to next candidate */
    }
}

seq_t a(idx_t n)
{
    /* We need primes p_{n} … p_{n+10}.  */
    size_t need = (size_t)n + 10;         /* number of primes required to satisfy p_{n+10} */
    ensure_prime_count(need);

    static const seq_t C10[11] = {
        (seq_t)1,   (seq_t)10,  (seq_t)45,  (seq_t)120,
        (seq_t)210, (seq_t)252, (seq_t)210, (seq_t)120,
        (seq_t)45,  (seq_t)10,  (seq_t)1
    };

    seq_t result = 0;
    for (int i = 0; i <= 10; ++i)
    {
        size_t idx          = (size_t)n - 1 + i;   /* 0‑based index of prime */
        seq_t prime_val     = (seq_t)prime_cache[idx];
        seq_t coeff         = C10[i];
        int  sign           = ((10 - i) & 1) ? -1 : 1;  /* (-1)^{10-i} */
        seq_t term          = coeff * prime_val;
        if (sign < 0)
            term = -term;
        result += term;
    }
    return result;
}
