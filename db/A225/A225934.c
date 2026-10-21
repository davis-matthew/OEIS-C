/*
 * A225934  Number of conjugacy classes in simply connected twisted Chevalley
 *          group 2E6(q) as q runs through the prime powers.
 * https://oeis.org/A225934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed from the n-th prime power q. For each residue
 *            of q mod 6 the value is a = q^6 + q^5 + 2q^4 + 4q^3 + C*q^2 +
 *            D*q + E where (C,D,E) depend on q mod 6: (C,D,E) = (11,11,16) if
 *            q≡1 (mod 6) (C,D,E) = (18,26,62) if q≡2 (mod 6) (C,D,E) =
 *            (11,11,15) if q≡3 (mod 6) (C,D,E) = (10,10,14) if...
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 285, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of comparator for qsort */
static int cmp_seq(const void *a, const void *b);

/* Static cache of prime powers */
static seq_t *primePowers = NULL;
static idx_t primePowersSize = 0;
static idx_t sieveLimit = 0;

/* Helper: generate all prime powers <= limit and store in primePowers */
static void recompute_prime_powers(idx_t limit)
{
    /* Sieve of Eratosthenes */
    bool *isPrime = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!isPrime) return;
    for (idx_t i = 2; i <= limit; ++i) isPrime[i] = true;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
        if (isPrime[i])
            for (idx_t j = i * i; j <= limit; j += i) isPrime[j] = false;

    /* Collect primes */
    size_t primeCnt = 0;
    size_t primeCap = limit / 10;
    uint32_t *primes = (uint32_t*)malloc(primeCap * sizeof(uint32_t));
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) {
            if (primeCnt == primeCap) {
                primeCap <<= 1;
                primes = (uint32_t*)realloc(primes, primeCap * sizeof(uint32_t));
            }
            primes[primeCnt++] = (uint32_t)i;
        }
    free(isPrime);

    /* Generate prime powers */
    size_t qCap = primeCnt * 4;
    seq_t *qList = (seq_t*)malloc(qCap * sizeof(seq_t));
    size_t qSize = 0;
    for (size_t pi = 0; pi < primeCnt; ++pi) {
        uint64_t p = primes[pi];
        seq_t power = (seq_t)p;
        while (power <= (seq_t)limit) {
            if (qSize == qCap) {
                qCap <<= 1;
                qList = (seq_t*)realloc(qList, qCap * sizeof(seq_t));
            }
            qList[qSize++] = power;
            power *= (seq_t)p;
        }
    }
    free(primes);

    /* Sort the list */
    qsort(qList, qSize, sizeof(seq_t), cmp_seq);

    /* Replace the old cache */
    free(primePowers);
    primePowers = qList;
    primePowersSize = (idx_t)qSize;
    sieveLimit = limit;
}

/* Comparator used by qsort to order seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t*)a;
    seq_t y = *(const seq_t*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Ensure we have enough prime powers cached */
    if (primePowers == NULL || n > primePowersSize) {
        idx_t limit = (sieveLimit >= 200000) ? sieveLimit * 2 : 200000;
        while (true) {
            recompute_prime_powers(limit);
            if ((idx_t)primePowersSize >= n) break;
            limit <<= 1;  /* double the limit and try again */
        }
    }

    /* Retrieve the n-th prime power (1‑based index). */
    seq_t q = primePowers[n - 1];

    /* Compute successive powers of q. */
    seq_t q2 = q * q;
    seq_t q3 = q2 * q;
    seq_t q4 = q3 * q;
    seq_t q5 = q4 * q;
    seq_t q6 = q5 * q;

    /* Determine constants based on q modulo 6. */
    int mod = (int)(q % 6);
    seq_t result;
    switch (mod) {
        case 1:
            result = q6 + q5 + q4 * 2 + q3 * 4 + q2 * 11 + q * 11 + 16;
            break;
        case 2:
            result = q6 + q5 + q4 * 2 + q3 * 4 + q2 * 18 + q * 26 + 62;
            break;
        case 3:
            result = q6 + q5 + q4 * 2 + q3 * 4 + q2 * 11 + q * 11 + 15;
            break;
        case 4:
            result = q6 + q5 + q4 * 2 + q3 * 4 + q2 * 10 + q * 10 + 14;
            break;
        case 5:
            result = q6 + q5 + q4 * 2 + q3 * 4 + q2 * 19 + q * 27 + 72;
            break;
        default: /* Should never happen for prime powers */
            result = 0;
            break;
    }
    return result;
}
