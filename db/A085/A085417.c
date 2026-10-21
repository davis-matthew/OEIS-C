/*
 * A085417  Take prime(n) and continue adding n,n+1,..., n+a(n)-1 until one
 *          reaches a prime.
 * https://oeis.org/A085417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest k ≥ 1 such that prime(n) + ∑_{i=0}^{k-1}
 *            (n+i) is prime. That is, starting from the n‑th prime, keep
 *            adding n, n+1, … until the running sum becomes prime. The value
 *            returned is the number of added terms (i.e. k)
 * Range:     u8 n <= 13, u16 n <= 4117, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"       /* uint32_t get_prime(idx_t k); */
#include "../utils/isPrimeGeneric.h"  /* bool isPrimeGeneric(seq_t n); */

seq_t a(idx_t n)
{
    /* n is 1‑based index. */
    if (n == 0) return 0;           /* undefined for n=0 */

    /* Start with the n‑th prime. */
    seq_t sum = (seq_t)get_prime(n); /* prime[n] */

    seq_t k = 0;          /* number of terms added so far */
    seq_t term = (seq_t)n;/* first term to add */

    /* Always add at least one term. */
    while (true) {
        sum += term;
        k += 1;
        if (isPrimeGeneric(sum))
            return k;
        term += 1;
    }
}
