/*
 * A037169  a(n) = prime(n) * Product_{k=0..n-2} prime(n-k) mod prime(n-k-1).
 * https://oeis.org/A037169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * ∏_{k=0..n-2} prime(n-k) mod prime(n-k-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */
static uint64_t *prime_buf = NULL;   /* array of first prime_cnt primes (0‑based) */
static size_t     prime_cnt = 0;     /* number of primes stored */
static size_t     sieve_limit = 0;   /* largest value up to which we have sieved */

/* ensure that prime_buf contains at least the first n primes */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    /* upper bound for the n-th prime: for n ≥ 6, 15n is enough
       (100 000th prime < 1.3 M), otherwise use a small constant                  */
    size_t limit = (n > 6) ? (size_t)(n * 15) : 15;
    if (limit < sieve_limit)
        limit = sieve_limit;

    /* simple sieve of Eratosthenes up to 'limit'                      */
    unsigned char *comp = (unsigned char*)calloc(limit + 1, sizeof *comp);
    if (!comp)
        abort();   /* out of memory */

    size_t cap = limit / 10 + 100;                 /* rough capacity estimate    */
    uint64_t *pr = (uint64_t*)realloc(prime_buf, cap * sizeof *pr);
    if (!pr)
        abort();
    prime_buf = pr;
    prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (prime_cnt >= cap) {
                cap <<= 1;                      /* double capacity          */
                prime_buf = (uint64_t*)realloc(prime_buf, cap * sizeof *prime_buf);
                if (!prime_buf)
                    abort();
            }
            prime_buf[prime_cnt++] = (uint64_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }

    free(comp);
    sieve_limit = limit;
}

/* Compute the n-th term of OEIS A037169. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1 */

    ensure_primes(n);

    seq_t res = (seq_t)prime_buf[(size_t)(n - 1)];   /* prime(n) */

    for (idx_t j = n - 1; j > 0; --j) {
        uint64_t larger  = prime_buf[(size_t)j];
        uint64_t smaller = prime_buf[(size_t)(j - 1)];
        seq_t resid = (seq_t)(larger % smaller);    /* prime(n‑k) mod prime(n‑k‑1) */
        res = res * resid;
    }

    return res;
}
