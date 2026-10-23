/*
 * A037167  a(n) = prime(n)*prime(n+1) - prime(n+1).
 * https://oeis.org/A037167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * prime(n+1) - prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helpers that may be provided by the build system.  They are not needed
   for the algorithm below, but we keep the headers in case they are
   required by the build environment. */
#ifdef __cplusplus
extern "C" {
#endif
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void abort(void);
#ifdef __cplusplus
}
#endif

/* Static state: dynamic array of primes in ascending order. */
static seq_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Ensure that primes[0 .. target] (inclusive) exist.
   target is the zero‑based index of the largest prime we need.
   The routine generates primes lazily using trial division. */
static void ensure_primes_upto(idx_t target)
{
    while (primes_cnt <= (size_t)target) {
        /* Compute the next candidate prime. */
        idx_t candidate = (primes_cnt == 0) ? 2 : (idx_t)primes[primes_cnt - 1] + 1;
        /* Search for the next prime by trial division using the primes
           we have already found. */
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                seq_t p = primes[i];
                idx_t p_val = (idx_t)p;
                if (p_val * p_val > candidate)
                    break;
                if (candidate % p_val == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            ++candidate;
        }

        /* Grow the array if necessary. */
        if (primes_cnt == primes_cap) {
            size_t new_cap = (primes_cap == 0) ? 2 : primes_cap * 2;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Out of memory; terminate the program. */
                abort();
            }
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_cnt++] = (seq_t)candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1 (offset 1). */

    /* We need prime(n) and prime(n+1).  Since primes is zero based,
       these correspond to indices n-1 and n respectively. */
    ensure_primes_upto((idx_t)n);

    seq_t p_n   = primes[(size_t)n - 1]; /* prime(n)   */
    seq_t p_np1 = primes[(size_t)n];     /* prime(n+1) */

    return p_np1 * p_n - p_np1;
}
