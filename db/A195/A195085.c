/*
 * A195085  Positive integers n for which there exist exactly two integers k
 *          in {1,2,3,...,n-1} such that k*n is square.
 * https://oeis.org/A195085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9 * (n-th squarefree number)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of 9 times all squarefree positive integers.
 * The n-th term can be obtained by computing the n-th squarefree
 * number and multiplying by 9.                                                */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static data reused between calls */
    static seq_t *squarefree = NULL;   /* array of squarefree numbers   */
    static size_t      count   = 0;     /* number of squarefree numbers  */
    static idx_t       limit   = 0;     /* maximum number processed      */

    if (n == 0)
        return (seq_t)0;  /* not used, but keeps function safe */

    /* Ensure we have at least n squarefree numbers. */
    while (count < n) {
        idx_t newLimit = limit ? limit * 2 : (idx_t)(n * 2) + 10;
        /* Allocate temporary arrays for primes and squarefree marks */
        bool *isPrime = (bool *)malloc((newLimit + 1) * sizeof(bool));
        bool *sqfree  = (bool *)malloc((newLimit + 1) * sizeof(bool));
        if (!isPrime || !sqfree) {
            /* Memory allocation failed – return 0 as a fallback */
            free(isPrime);
            free(sqfree);
            return (seq_t)0;
        }

        /* Initialize prime sieve */
        memset(isPrime, 1, (newLimit + 1) * sizeof(bool));
        isPrime[0] = isPrime[1] = false;
        for (idx_t p = 2; p * p <= newLimit; ++p) {
            if (isPrime[p]) {
                for (idx_t j = p * p; j <= newLimit; j += p)
                    isPrime[j] = false;
            }
        }

        /* Mark numbers that are divisible by a square of a prime */
        memset(sqfree, 1, (newLimit + 1) * sizeof(bool));
        sqfree[0] = false;
        for (idx_t p = 2; p * p <= newLimit; ++p) {
            if (isPrime[p]) {
                idx_t square = p * p;
                for (idx_t j = square; j <= newLimit; j += square)
                    sqfree[j] = false;
            }
        }

        /* Reallocate and fill the squarefree list up to newLimit. */
        size_t newCount = 0;
        size_t newCap   = count * 2 + 16;
        seq_t *newArray = (seq_t *)realloc(squarefree, newCap * sizeof(seq_t));
        if (!newArray) {
            free(isPrime);
            free(sqfree);
            return (seq_t)0;
        }
        squarefree = newArray;

        for (idx_t i = 1; i <= newLimit; ++i) {
            if (sqfree[i]) {
                if (newCount < newCap) {
                    squarefree[newCount] = (seq_t)i;
                    ++newCount;
                }
            }
        }

        free(isPrime);
        free(sqfree);

        count = newCount;
        limit = newLimit;

        /* If we still don't have enough, double the limit and repeat. */
        if (count < n)
            continue;
    }

    /* Return the n-th term: 9 times the (n-1)-th squarefree number. */
    return (seq_t)9 * squarefree[n - 1];
}
