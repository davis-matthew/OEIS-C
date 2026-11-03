/*
 * A164534  (n-th isolated prime) -(-1)^(n-th isolated prime).
 * https://oeis.org/A164534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (n-th isolated prime) - (-1)^(n-th isolated prime)
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: recompute isolated primes up to new_limit */
static void recompute_isolated_primes(size_t new_limit, seq_t **out_arr, size_t *cnt)
{
    /* Simple sieve for all numbers up to new_limit */
    bool *isPrime = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!isPrime) return; /* abort on bad alloc */
    for (size_t i = 0; i <= new_limit; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                isPrime[j] = false;
        }
    }

    /* count isolated primes: p-2 and p+2 must be composite (or p==2) */
    size_t isolated_cnt = 0;
    for (size_t p = 2; p <= new_limit; ++p) {
        if (isPrime[p]) {
            if (p == 2 || (p > 2 && !isPrime[p - 2] && !isPrime[p + 2]))
                ++isolated_cnt;
        }
    }

    /* allocate array for isolated primes */
    seq_t *isolated = mem_new(seq_t, isolated_cnt);
    size_t idx = 0;
    for (size_t p = 2; p <= new_limit; ++p) {
        if (isPrime[p]) {
            if (p == 2 || (p > 2 && !isPrime[p - 2] && !isPrime[p + 2]))
                isolated[idx++] = (seq_t)p;
        }
    }

    free(isPrime);
    *out_arr = isolated;
    *cnt = isolated_cnt;
}

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
            mem_free(isolatedArr);
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
