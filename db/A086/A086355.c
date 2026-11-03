/*
 * A086355  Fixed point if [nonzero-digit product]-function at
 *          initial-value=prime(n) is iterated.
 * https://oeis.org/A086355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = fixed point of the non‑zero digit product * of prime(n)
 *            (i.e. A051802(A000040(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9887 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return the product of the non‑zero decimal digits of x
 * (computed with 64‑bit arithmetic). */
static uint64_t digit_prod_nonzero(uint64_t x)
{
    uint64_t prod = 1;
    while (x > 0) {
        uint64_t d = x % 10;
        if (d != 0) prod *= d;
        x /= 10;
    }
    return prod;
}

/* Generate at least the first n primes, return a pointer to a static
 * array of 64‑bit primes.  The produced primes are stored in a
 * dynamically expanding array.  This routine is safe to call
 * repeatedly and in any order. */
static uint64_t *ensure_primes(idx_t n)
{
    /* static prime list */
    static uint64_t *primes = NULL;
    static idx_t   size   = 0;   /* number of primes stored */
    static idx_t   cap    = 0;   /* allocated capacity */

    if (size >= n) return primes;   /* already have enough */

    if (primes == NULL) {           /* first time */
        cap = 16;
        primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
        if (!primes) return NULL;   /* allocation failure – abort silently */
    }

    /* start checking from last prime + 1 (or 2 if list empty) */
    idx_t cand = (size > 0) ? (idx_t)(primes[size - 1] + 1) : (idx_t)2;

    while (size < n) {
        bool is_prime = true;
        for (idx_t i = 0; i < size; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;     /* no need to test further */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (size == cap) {            /* grow array */
                cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp) {               /* realloc failed – abort silently */
                    free(primes);
                    primes = NULL;
                    return NULL;
                }
                primes = tmp;
            }
            primes[size++] = (uint64_t)cand;
        }
        ++cand;
    }
    return primes;
}

/* Sequence definition */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    uint64_t *prime_list = ensure_primes(n);
    if (!prime_list) return (seq_t)0;     /* allocation error – abort silently */

    uint64_t cur = prime_list[n - 1];     /* nth prime (1‑based) */

    /* iterate digit product until a fixed point is reached */
    for (;;) {
        uint64_t next = digit_prod_nonzero(cur);
        if (next == cur) break;
        cur = next;
    }
    return (seq_t)cur;                     /* final fixed point is a small value */
}
