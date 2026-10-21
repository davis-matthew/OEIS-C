/*
 * A066872  a(n) = prime(n)^2 + 1.
 * https://oeis.org/A066872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)^2 + 1
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th term of OEIS A066872.
    Generates primes on demand with an incremental trial‑division
    algorithm.  The routine is self‑contained and works for
    any idx_t >= 1.  */
#include <stdlib.h>
#include <stdint.h>

/* Buffer to store primes that have already been found */
static seq_t *primes = NULL;     /* dynamic array of primes */
static size_t primes_len = 0;    /* number of primes stored */
static size_t primes_cap = 0;    /* current capacity */

/* Ensure that at least n primes have been generated.
   n is 1‑based; the function stops as soon as primes_len >= n. */
static void ensure_primes(idx_t n)
{
    if (primes_len >= (size_t)n)
        return;

    /* Grow the array incrementally as needed. */
    if (primes == NULL) {
        primes_cap = 128;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes)
            return;   /* allocation failed – caller will get 0 */
    }

    /* If no primes yet, add the first prime 2. */
    if (primes_len == 0)
        primes[primes_len++] = (seq_t)2;

    /* Candidate starts from the first odd > 2. */
    seq_t candidate = (primes_len == 1) ? (seq_t)3 : primes[primes_len - 1] + (seq_t)2;

    while (primes_len < (size_t)n) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_len; ++i) {
            seq_t p = primes[i];
            /* stop when p*p > candidate                           */
            if ((p * p) > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;   /* double capacity */
                primes = (seq_t *)realloc(primes,
                                          primes_cap * sizeof(seq_t));
                if (!primes)
                    return;   /* allocation failed */
            }
            primes[primes_len++] = candidate;
        }
        candidate += (seq_t)2;   /* only odd candidates */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* the sequence is offset 1 – n=0 is undefined */
    ensure_primes(n);
    seq_t p = primes[n - 1];
    return (p * p) + (seq_t)1;
}
