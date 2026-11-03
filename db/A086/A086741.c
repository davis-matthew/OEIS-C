/*
 * A086741  a(n) = A026905(n) - A014284(n).
 * https://oeis.org/A086741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086741: a(n) = A026905(n) – A014284(n)
   A026905(n) = Σ_{k=1..n} p(k)   (p(k) = partition number)
   A014284(n) = Σ_{k=1..n} q(k)   (q(1)=1, q(k>1)=k‑th ordinary prime) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mem.h"

extern seq_t a(idx_t n)
{
    /* ----------------------------------------------------------- */
    /* 1.  Partial sums of partition numbers (A026905)            */
    /* ----------------------------------------------------------- */
    static seq_t *pVal   = NULL;        /* partition numbers p(i)  */
    static seq_t *pSum   = NULL;        /* partial sums Σ_{k=1..i} p(k) */
    static size_t pSize  = 0;           /* number of stored elements   */

    if (pSize == 0) {
        pSize = 1;                      /* store p(0) and pSum[0] only  */
        pVal = mem_new(seq_t, 1);
        pSum = mem_new(seq_t, 1);
        pVal[0] = (seq_t)1;             /* p(0)=1 for recurrence      */
        pSum[0] = (seq_t)0;             /* sum for zero elements       */
    }

    if (pSize <= (size_t)n) {
        size_t oldSize = pSize;
        pSize = (size_t)n + 1;
        pVal = mem_resize(seq_t, pVal, pSize);
        pSum = mem_resize(seq_t, pSum, pSize);
        for (size_t i = oldSize; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)0;
            for (int k = 1;; ++k) {
                size_t pent1 = ((size_t)k * ((size_t)3 * k - 1)) / 2;
                if (pent1 > i) break;
                if (k & 1)    val += pVal[i - pent1];
                else          val -= pVal[i - pent1];
                size_t pent2 = ((size_t)k * ((size_t)3 * k + 1)) / 2;
                if (pent2 > i) continue;
                if (k & 1)    val += pVal[i - pent2];
                else          val -= pVal[i - pent2];
            }
            pVal[i] = val;
            pSum[i] = (i == 0) ? (seq_t)0 : (pSum[i - 1] + val);
        }
    }

    /* ----------------------------------------------------------- */
    /* 2.  Partial sums of primes with 1 considered as a prime   */
    /* ----------------------------------------------------------- */
    static seq_t *primeSum  = NULL;      /* Σ_{k=1..i} q(k)          */
    static size_t primeSumSize = 0;      /* number of stored sums    */
    static uint64_t *primeList = NULL;  /* ordinary primes start at 2 */
    static size_t primeListSize = 0;
    static size_t primeListCap  = 0;
    static uint64_t nextCandidate = 2;   /* next integer to test      */

    if (primeSumSize == 0) {
        primeSumSize = 2;                /* indices 0 and 1 exist    */
        primeSum = mem_new(seq_t, primeSumSize);
        primeSum[0] = (seq_t)0;           /* Σ for zero elements      */
        primeSum[1] = (seq_t)1;           /* q(1) = 1                 */
    }

    while (primeSumSize <= (size_t)n) {
        bool isPrime = true;
        uint64_t limit = (uint64_t) sqrt((double)nextCandidate);
        for (size_t i = 0; i < primeListSize; ++i) {
            uint64_t p = primeList[i];
            if (p > limit) break;
            if (nextCandidate % p == 0) { isPrime = false; break; }
        }
        if (isPrime) {
            /* extend primeList */
            if (primeListSize == primeListCap) {
                primeListCap = primeListCap ? primeListCap * 2 : 8;
                primeList = (uint64_t *)realloc(primeList, primeListCap * sizeof(uint64_t));
            }
            primeList[primeListSize++] = nextCandidate;

            /* extend primeSum */
            seq_t newSum = primeSum[primeSumSize - 1] + (seq_t)nextCandidate;
            primeSum = mem_resize(seq_t, primeSum, (primeSumSize + 1));
            primeSum[primeSumSize] = newSum;
            ++primeSumSize;
        }
        ++nextCandidate;
    }

    /* ----------------------------------------------------------- */
    /* 3.  Difference of the two partial sums                      */
    /* ----------------------------------------------------------- */
    return pSum[n] - primeSum[n];
}
