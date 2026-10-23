/*
 * A127908  Primes of form (3^n + 2^n)/5.
 * https://oeis.org/A127908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^k + 3^k) / 5 where k is the n-th element of A057469 (k
 *            runs over the odd indices 3,7,11,83,149,…)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/is_prime_big.h"

/* Cached primes of the sequence */
static seq_t *prime_vals = NULL;   /* array of a(i) values */
static size_t  prime_cnt = 0;       /* number of cached values */
static size_t  prime_cap = 0;       /* capacity of the cache */

/* Current odd exponent being examined (k in the description) */
static idx_t   curr_exp   = 3;     /* start with k = 3 */

/* Current powers 2^curr_exp and 3^curr_exp */
static seq_t   pow2_val   = 0;
static seq_t   pow3_val   = 0;

/* Append a new prime to the cache, expanding it if necessary. */
static void push_prime(seq_t val)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 64 : prime_cap << 1;
        seq_t *tmp = (seq_t *)realloc(prime_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        prime_vals = tmp;
        prime_cap  = new_cap;
    }
    prime_vals[prime_cnt++] = val;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* OEIS offset starts at 1; behaviour for n=0 is undefined. */
        return (seq_t)0;
    }

    /* Lazy initialisation of the cache and first values. */
    if (prime_vals == NULL) {
        prime_cap = 64;
        prime_vals = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!prime_vals) abort();   /* allocation failed */
        pow2_val = pow2(3);         /* 2^3 */
        pow3_val = pow3(3);         /* 3^3 */
        curr_exp  = 3;
    }

    /* Generate further terms until we have at least n terms cached. */
    while (prime_cnt < (size_t)n) {
        seq_t sum = pow2_val + pow3_val;   /* 2^k + 3^k */
        seq_t val = sum / (seq_t)5;        /* division is exact for odd k */

        if (is_prime_big(val)) {
            push_prime(val);
        }

        /* Advance to the next odd exponent: k += 2.
           2^(k+2) = 2^k * 4  and  3^(k+2) = 3^k * 9.   */
        curr_exp += 2;
        pow2_val = pow2_val * (seq_t)4;
        pow3_val = pow3_val * (seq_t)9;
    }

    return prime_vals[n-1];   /* zero‑based array */
}
