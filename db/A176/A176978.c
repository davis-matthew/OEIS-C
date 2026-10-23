/*
 * A176978  Numbers n such that n^2 + 13 and n^2 + 13^2 are primes.
 * https://oeis.org/A176978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 724, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n^2 + 13 and n^2 + 13^2 are primes.
   The function returns the nth such integer (1‑based). */

#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Cache of found terms.  */
static seq_t *terms = NULL;
static idx_t count = 0;          /* number of cached terms */
static idx_t cap   = 0;          /* allocated capacity of 'terms' */
static idx_t last_candidate = 0; /* last candidate n tested */

/* Return true if x is prime, choosing the fastest routine available. */
static inline bool is_prime_check(seq_t x)
{
    if (sizeof(seq_t) <= 8) {
        return is_prime_small((uint64_t)x);
    } else {
        return is_prime_big(x);
    }
}

/* Ensure that 'terms' can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (cap >= needed)
        return;
    idx_t newcap = cap ? cap << 1 : 4;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* in production, report error */
    }
    terms = tmp;
    cap   = newcap;
}

/* Return the n‑th term of A176978 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* sequence defined for n ≥ 1; 0 is a dummy */

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        last_candidate++;                     /* next integer to test */
        seq_t c  = (seq_t)last_candidate;     /* candidate n      */
        seq_t c2 = c * c;                     /* n^2              */

        seq_t p1 = c2 + (seq_t)13;            /* n^2 + 13        */
        seq_t p2 = c2 + (seq_t)169;           /* n^2 + 13^2      */

        if (is_prime_check(p1) && is_prime_check(p2)) {
            ensure_capacity(count + 1);
            terms[count++] = c;
        }
    }

    return terms[n - 1];  /* 1‑based indexing */
}
