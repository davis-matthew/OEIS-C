/*
 * A176412  Concatenation of prime Lucas numbers written in base 2.
 * https://oeis.org/A176412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = n-th bit of the concatenation of the binary
 *            representations of the prime Lucas numbers (in order). Indexing
 *            starts at 1. The function builds a minimal memoisation of prime
 *            Lucas numbers until enough bits have been accumulated
 * Range:     u8 n <= 32, u16 n <= 83, u32 n <= 81, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

/* Return the nth term of OEIS sequence A176412 */
seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *prime_vals      = NULL;   /* prime Lucas values    */
    static size_t *prime_bitlen   = NULL;   /* bit length of each   */
    static size_t *prime_cumlen   = NULL;   /* cumulative bits so far */
    static size_t prime_count     = 0;       /* number of primes found */
    static size_t prime_cap       = 0;       /* allocated capacity     */

    /* state for generating Lucas numbers */
    static seq_t L_prev2 = (seq_t)2;   /* L(0) */
    static seq_t L_prev1 = (seq_t)1;   /* L(1) */

    /* initialise on first call */
    if (prime_count == 0) {
        prime_cap = 16;
        prime_vals   = (seq_t*)   malloc(prime_cap * sizeof(seq_t));
        prime_bitlen = (size_t*) malloc(prime_cap * sizeof(size_t));
        prime_cumlen = (size_t*) malloc(prime_cap * sizeof(size_t));

        /* first prime Lucas number is L(0)=2 */
        prime_vals[0]   = (seq_t)2;
        prime_bitlen[0] = bitlen_seq_t(prime_vals[0]);
        prime_cumlen[0] = prime_bitlen[0];
        prime_count = 1;
    }

    size_t target = (size_t)n; /* n is guaranteed ≥1 */

    /* extend memoisation until we have at least 'target' bits */
    while (prime_cumlen[prime_count - 1] < target) {
        /* generate next Lucas number */
        seq_t L_next = L_prev1 + L_prev2;
        L_prev2 = L_prev1;
        L_prev1 = L_next;

        /* if it is prime, store it */
        if (is_prime_big(L_next)) {
            unsigned bitlen = bitlen_seq_t(L_next);

            if (prime_count == prime_cap) {
                size_t newcap = prime_cap * 2;
                prime_vals   = (seq_t*)
                    realloc(prime_vals,   newcap * sizeof(seq_t));
                prime_bitlen = (size_t*)
                    realloc(prime_bitlen, newcap * sizeof(size_t));
                prime_cumlen = (size_t*)
                    realloc(prime_cumlen, newcap * sizeof(size_t));
                prime_cap = newcap;
            }

            prime_vals[prime_count]   = L_next;
            prime_bitlen[prime_count] = bitlen;
            prime_cumlen[prime_count] =
                prime_cumlen[prime_count - 1] + bitlen;
            ++prime_count;
        }
    }

    /* binary search for the prime that contains the target bit */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_cumlen[mid] >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t idx     = lo;
    size_t cumprev = (idx == 0) ? 0 : prime_cumlen[idx - 1];
    size_t offset  = target - cumprev - 1;            /* 0‑based within this number */
    size_t bitlen  = prime_bitlen[idx];
    size_t shift   = bitlen - 1 - offset;
    seq_t  val     = prime_vals[idx];

    return (val >> shift) & (seq_t)1;
}
