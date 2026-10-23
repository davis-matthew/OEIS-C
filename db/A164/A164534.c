/*
 * A164534  (n-th isolated prime) -(-1)^(n-th isolated prime).
 * https://oeis.org/A164534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (n-th isolated prime) - (-1)^(n-th isolated prime)
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   recompute_isolated_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/recompute_isolated_primes.h"

/* static storage for isolated primes */
static seq_t *isolatedArr = NULL;
static size_t isolatedCnt = 0;
static size_t sieveLimit = 0;

/* ensure we have at least n isolated primes computed */
static void ensure_isolated_primes(idx_t n)
{
    if ((size_t)n <= isolatedCnt)
        return;

    size_t limit = sieveLimit ? sieveLimit * 2 : 1000;
    while (1) {
        seq_t *newArr = NULL;
        size_t newCnt = 0;
        recompute_isolated_primes(limit, &newArr, &newCnt);
        if (newArr) {
            free(isolatedArr);
            isolatedArr = newArr;
            isolatedCnt = newCnt;
            sieveLimit = limit;
            if ((size_t)n <= isolatedCnt)
                return;
            limit *= 2;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* sequence starts at offset 1; n>=1 per specification */
    ensure_isolated_primes(n);
    seq_t p = isolatedArr[n - 1];          /* 0‑based index */
    if ((p & 1) == 0)                       /* p even (only p=2) */
        return p - 1;
    else
        return p + 1;                       /* odd isolated primes */
}
