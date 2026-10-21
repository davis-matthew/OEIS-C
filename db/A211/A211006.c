/*
 * A211006  Pair (n,p) where n is the sum of adjacent nonprimes and p is the
 *          sum of adjacent primes.
 * https://oeis.org/A211006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A211006: Pair (n,p) where n is the sum of adjacent nonprimes
   and p is the sum of adjacent primes.
   
   Computation:
     * a(1)=1, a(2)=5, a(3)=4, a(5)=6
     * For even n >= 4: a(n) = p_{n/2+1}, where p_i is the i‑th prime.
     * For odd n >= 7: a(n) = Σ_{k=p_g+1}^{p_{g+1}-1} k,
       with g = (n+1)/2  (the index of the first prime in the pair).
   The implementation precomputes enough primes to support n up to 10 000.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* auxiliary prime data */
static size_t prime_count = 0;
static uint64_t primes[20000];   /* ample for the needed range */

/* one‑time prime sieve */
static void init_primes(void)
{
    if (prime_count > 0) return;            /* already initialized */

    const size_t LIMIT = 120000;            /* > 1st 6000 primes */
    uint8_t *is_p = (uint8_t *)malloc(LIMIT + 1);
    if (!is_p) return;                       /* should never happen */

    memset(is_p, 1, LIMIT + 1);
    is_p[0] = is_p[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_p[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_p[j] = 0;

    for (size_t i = 2; i <= LIMIT && prime_count < 20000; ++i)
        if (is_p[i])
            primes[prime_count++] = i;

    free(is_p);
}

/* main sequence generator */
seq_t a(idx_t n)
{
    init_primes();                            /* ensure primes ready */

    uint64_t N = (uint64_t)n;

    /* special small indices */
    if (N == 0) return (seq_t)0;
    if (N == 1) return (seq_t)1;
    if (N == 2) return (seq_t)5;
    if (N == 3) return (seq_t)4;
    if (N == 5) return (seq_t)6;

    /* even indices (n >= 4) */
    if ((N & 1ULL) == 0) {
        uint64_t k = N / 2 + 1;                /* prime index */
        if (k == 0 || k > prime_count) return (seq_t)0;
        return (seq_t)primes[k - 1];
    }

    /* odd indices (n >= 7) */
    uint64_t g = (N + 1ULL) / 2;              /* index of the first prime in the pair */
    if (g == 0 || g + 1 > prime_count) return (seq_t)0;

    uint64_t p1 = primes[g - 1];
    uint64_t p2 = primes[g];

    if (p2 <= p1 + 1) return (seq_t)0;       /* no composites between */

    uint64_t first = p1 + 1;
    uint64_t last  = p2 - 1;
    uint64_t cnt   = last - first + 1;

    /* arithmetic series sum: (first + last) * cnt / 2 */
    unsigned __int128 sum = (unsigned __int128)(first + last) * cnt / 2;
    return (seq_t)sum;
}
