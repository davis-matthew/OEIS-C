/*
 * A074820  Numbers k such that mu(k) = mu(k+2), where mu is the Möbius
 *          function (A008683).
 * https://oeis.org/A074820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that μ(k) = μ(k+2)
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a linear sieve for the Möbius function,
   then scans for the n‑th k satisfying the condition. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static size_t MU_SIZE = 0;
static int8_t *MU = NULL;          /* Möbius values */
static int *PRIMES = NULL;         /* list of primes */
static size_t PRIMES_CNT = 0;

/* Build or extend the Möbius sieve up to `limit` (inclusive). */
static void
prepare_sieve(size_t limit)
{
    if (limit <= MU_SIZE)
        return;               /* already built */

    /* Free previous data */
    free(MU);
    free(PRIMES);

    MU = (int8_t *)calloc(limit + 1, sizeof(int8_t));
    PRIMES = (int *)malloc((limit + 1) * sizeof(int));

    MU[1] = 1;
    PRIMES_CNT = 0;

    /* Linear sieve for μ */
    for (size_t i = 2; i <= limit; ++i) {
        if (!PRIMES[i]) {                /* i is prime if not marked yet */
            PRIMES[i] = 1;                /* Mark as prime */
            PRIMES[PRIMES_CNT++] = (int)i;
            MU[i] = -1;
        }
        for (size_t j = 0; j < PRIMES_CNT; ++j) {
            size_t p = (size_t)PRIMES[j];
            size_t prod = i * p;
            if (prod > limit)
                break;
            PRIMES[prod] = 1;            /* Mark as composite */
            if (i % p == 0) {
                MU[prod] = 0;            /* Square factor */
                break;
            } else {
                MU[prod] = -MU[i];
            }
        }
    }

    MU_SIZE = limit;
}

/* Return the k-th number such that μ(k)=μ(k+2) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* Undefined for n=0, but return 0 */

    /* Estimate a sufficient upper bound for scanning */
    size_t upper = (size_t)(n * 5 + 1000);
    if (upper < 500000)        /* we know 10000th term < 500000 */
        upper = 500000;
    upper += 2;                /* to be safe for k+2 access */

    /* Build sieve */
    prepare_sieve(upper);

    size_t count = 0;
    uint64_t answer = 0;
    for (uint64_t k = 1; k + 2 <= upper; ++k) {
        if (MU[k] == MU[k + 2]) {
            ++count;
            if (count == (size_t)n) {
                answer = k;
                break;
            }
        }
    }

    /* If not found (unlikely for n <= 10000), extend the sieve */
    if (count < (size_t)n) {
        /* Double the range and rebuild */
        size_t new_upper = upper * 2;
        prepare_sieve(new_upper);
        count = 0;
        for (uint64_t k = 1; k + 2 <= new_upper; ++k) {
            if (MU[k] == MU[k + 2]) {
                ++count;
                if (count == (size_t)n) {
                    answer = k;
                    break;
                }
            }
        }
    }

    return (seq_t)answer;
}
