/*
 * A193315  Write 2n=j+q (j,q positive noncomposite numbers); j*q maximal;
 *          then a(n)=j*q.
 * https://oeis.org/A193315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ j*(2n-j) : j and 2n-j are positive noncomposite
 *            numbers } (a noncomposite number is either 1 or a prime). The
 *            function returns that maximal product for any n >= 1
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: dynamic sieve of Eratosthenes to test small numbers for primality. */
static unsigned char *prime_flags = NULL;   /* 0 = composite, 1 = prime */
static idx_t prime_limit = 0;              /* largest number sieved so far */

static void ensure_prime_limit(idx_t limit) {
    if (limit <= prime_limit) return;

    /* Rebuild sieve up to 'limit' */
    unsigned char *new_flags = (unsigned char*)calloc((size_t)limit + 1, 1);
    if (!new_flags) return;               /* allocation failure – should never happen */

    for (idx_t i = 0; i <= limit; ++i) new_flags[i] = 1;
    if (limit >= 0) new_flags[0] = 0;
    if (limit >= 1) new_flags[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_flags[p]) {
            for (idx_t v = p * p; v <= limit; v += p) new_flags[v] = 0;
        }
    }

    free(prime_flags);
    prime_flags = new_flags;
    prime_limit = limit;
}

/* Test whether a small unsigned integer is noncomposite (i.e. 1 or prime). */
static inline bool is_noncomposite(idx_t x) {
    return (x == 1) || (prime_flags && prime_flags[x]);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but avoid UB */
    idx_t two_n = n + n;         /* 2 * n, fits within idx_t for all constraints */
    ensure_prime_limit(two_n);

    idx_t half = two_n >> 1;     /* floor(2n/2) */
    for (idx_t j = half; j > 0; --j) {
        if (is_noncomposite(j)) {
            idx_t q = two_n - j;
            if (is_noncomposite(q)) {
                /* j and q are noncomposite and sum to 2n; earliest such pair
                   yields the maximum product because j*(2n-j) decreases as j moves
                   away from n. */
                return (seq_t)j * (seq_t)q;
            }
        }
    }
    /* Fallback – should never happen: for n >= 1 there is at least the pair (1,1). */
    return (seq_t)1;
}
