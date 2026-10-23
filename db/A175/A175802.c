/*
 * A175802  a(n) = 2^(positive nonprime(n)-1) mod (positive nonprime(n+1)).
 * https://oeis.org/A175802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2^(A018252(n)-1) mod A018252(n+1) (OEIS A175802)
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Return the nth term of A175802 (offset 1).  n is a (1 …)
 * positive integer. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset 1: n==0 unused */
        return 0;

    idx_t count = 0;                /* number of non‑primes seen so far */
    idx_t cand  = 1;                /* current integer being tested */

    idx_t m = 0, p = 0;             /* A018252(n) and A018252(n+1) */

    /* Scan the sequence of positive non‑primes once, stopping when
     * we have obtained both the n‑th and the (n+1)‑th terms. */
    while (1) {
        if (cand == 1 || !is_prime(cand)) {
            ++count;                /* we found a non‑prime */
            if (count == n)
                m = cand;           /* n‑th non‑prime */
            if (count == n + 1) {   /* (n+1)‑th non‑prime */
                p = cand;
                break;
            }
        }
        ++cand;
    }

    /* Compute 2^(m-1) modulo p.  m−1 fits in uint64_t for all
     * n requested (<= 10 000). */
    return mod_pow((seq_t)2, (uint64_t)(m > 0 ? m - 1 : 0), (seq_t)p);
}
