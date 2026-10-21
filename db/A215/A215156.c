/*
 * A215156  Primes congruent to {2, 3, 5, 7} mod 17.
 * https://oeis.org/A215156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence consists of the primes congruent to 2, 3, 5, 7 (mod 17). */
/*  The function generates primes on demand, keeps the ones that satisfy */
/*  the congruence in a dynamic array, and returns the n‑th element.   */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache of primes that meet the congruence requirement.  */
static seq_t *buf = NULL;   /* array of primes found */
static size_t cap = 0;      /* current buffer capacity   */
static size_t count = 0;    /* number of stored primes   */
static seq_t last = (seq_t)1; /* last tested candidate */

/* Return the n-th prime that ≡ 2, 3, 5, or 7 (mod 17);   n ≥ 1.   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for n=0 */

    /* Lazy initialization of cache. */
    if (buf == NULL) {
        cap = 16;
        buf = seq_alloc(cap);
        if (!buf) abort();
    }

    /* Generate primes until we have at least n of them. */
    while (count < (size_t)n) {
        /* Advance to next integer candidate. */
        ++last;

        /* Quick primality test. */
        if (!is_prime_big(last))
            continue;

        /* Check the required residue modulo 17. */
        seq_t r = last % (seq_t)17;
        if (r == (seq_t)2 || r == (seq_t)3 ||
            r == (seq_t)5 || r == (seq_t)7) {

            /* Resize the buffer if full. */
            if (count >= cap) {
                size_t new_cap = cap << 1;
                seq_t *tmp = seq_realloc(buf, cap, new_cap);
                if (!tmp) abort();
                buf = tmp;
                cap = new_cap;
            }

            buf[count++] = last;
        }
    }

    return buf[n - 1];
}
