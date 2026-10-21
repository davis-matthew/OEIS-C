/*
 * A061725  a(n) = p^2 + 2 where p is the n-th prime.
 * https://oeis.org/A061725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = p_n^2 + 2 where p_n is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS sequence A061725.
 * It lazily builds a list of primes on demand and reuses it
 * for subsequent calls, so the computation is O(n) in the number
 * of distinct primes requested. The implementation is purely
 * integer arithmetic and works with any seq_t/idx_t width. */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for primes */
static idx_t *prime_list = NULL;   /* array of extracted primes */
static size_t prime_count = 0;     /* number of primes currently stored */
static size_t prime_cap   = 0;     /* allocated capacity */

/* grow the prime list until it holds at least *need* primes */
static void grow_primes_until(size_t need)
{
    /* initialise storage if this is the first call */
    if (prime_cap == 0) {
        prime_cap   = 16;
        prime_list  = (idx_t*)malloc(prime_cap * sizeof(idx_t));
        if (!prime_list) exit(1);   /* simple error handling */
        prime_list[0] = 2;
        prime_count   = 1;
    }

    /* generate further primes until we have enough */
    for (idx_t cand = prime_list[prime_count-1] + 1;
         prime_count < need;
         ++cand)
    {
        /* skip even numbers – 2 is already stored */
        if ((cand & 1) == 0) continue;

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            idx_t p = prime_list[i];
            if (p * p > cand) break;          /* no need to test larger primes */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            /* enlarge array if necessary */
            if (prime_count == prime_cap)
            {
                size_t new_cap = prime_cap * 2;
                idx_t *tmp = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = cand;
        }
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset is 1; undefined below */

    /* Make sure we have at least n primes available */
    grow_primes_until((size_t)n);

    seq_t p = (seq_t)prime_list[n-1];     /* n‑th prime */
    return p * p + (seq_t)2;
}
