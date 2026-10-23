/*
 * A158337  Composite numbers k such that k - (number of prime factors of k,
 *          counted with multiplicity) is a prime.
 * https://oeis.org/A158337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 6319, u32 n <= 6427, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158337
 * Composite numbers k such that k - Ω(k) is a prime,
 * where Ω(k) is the number of prime factors of k counted
 * with multiplicity.  The first terms are
 * 4, 8, 9, 15, 20, 21, 25, ...
 * The function returns the n-th term (a(1)=4).
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* returns the n-th member of A158337 for n >= 1. */
seq_t a(idx_t n)
{
    /* static cache shared between calls */
    static seq_t *cache = NULL;
    static size_t len = 0;     /* number cached */
    static size_t cap = 0;     /* allocated size */
    static idx_t next_k = 4;   /* next candidate to examine */

    /* Grow cache if necessary. */
    if (len < n) {
        /* Ensure enough space for new entries. */
        grow_seq((seq_t **)&cache, &cap, n);
        for (; len < n; ++next_k) {
            /* skip primes (Ω = 1) and numbers < 4. */
            seq_t omega = bigomega(next_k);
            if (omega < 2) continue; /* not composite */

            seq_t m = (seq_t)next_k - omega;
            if (is_prime_big(m)) {
                cache[len++] = next_k;
            }
        }
    }
    return cache[n - 1];
}
