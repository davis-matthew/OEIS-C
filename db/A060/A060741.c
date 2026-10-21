/*
 * A060741  Number of divisors of 2n which are also differences between
 *          consecutive divisors of 2n (ordered by size).
 * https://oeis.org/A060741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(2n) - A060764(2n)
 * Range:     u8 n <= 9378, u16 n <= 9378, u32 n <= 9378, u64 n <= 9378,
 *            u128 n <= 9378, big any n
 * Helpers:   cmp_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the divisors of 2n that also appear as differences between
   consecutive divisors of 2n.  The algorithm factors 2n, enumerates
   all its divisors, computes successive differences, and then
   counts how many divisors are contained in the difference set. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_u64.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n must be >= 1. For n == 0 we return 0. */
    if (n == 0) return 0;

    /* Work in uint64_t – 2n fits easily for n up to many millions. */
    uint64_t M = (uint64_t)n * 2ULL;      /* 2n */

    /* ---------------------------------------------------------- */
    /* 1. Factorize M into primes and exponents.                */
    /* ---------------------------------------------------------- */
    uint64_t primes[20];
    int      exps[20];
    int      pcnt = 0;

    uint64_t temp = M;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0) continue;
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[pcnt]  = p;
        exps[pcnt]    = e;
        ++pcnt;
    }
    if (temp > 1) {          /* remaining prime factor */
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* ---------------------------------------------------------- */
    /* 2. Generate all divisors of M.                           */
    /* ---------------------------------------------------------- */
    /* 2n <= 20000 for n <= 10000, so a static array of 512 is safe. */
    uint64_t divs[512];
    size_t   dcnt = 1;           /* we always have divisor 1 */
    divs[0] = 1;

    for (int i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];
        /* powers of p */
        uint64_t pow[20];
        pow[0] = 1;
        for (int k = 1; k <= e; ++k) pow[k] = pow[k - 1] * p;

        size_t olddcnt = dcnt;
        for (size_t j = 0; j < olddcnt; ++j) {
            uint64_t base = divs[j];
            /* multiply by every non‑trivial power of p */
            for (int k = 1; k <= e; ++k) {
                divs[dcnt++] = base * pow[k];
            }
        }
    }

    /* ---------------------------------------------------------- */
    /* 3. Sort divisors and compute differences.                */
    /* ---------------------------------------------------------- */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_u64);

    size_t diffcnt = (dcnt > 0) ? dcnt - 1 : 0;
    uint64_t diffs[512];
    for (size_t i = 1; i < dcnt; ++i) diffs[i - 1] = divs[i] - divs[i - 1];
    qsort(diffs, diffcnt, sizeof(uint64_t), cmp_u64);

    /* ---------------------------------------------------------- */
    /* 4. Count divisors that also appear as a difference.      */
    /* ---------------------------------------------------------- */
    size_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if (diffcnt == 0) break;
        uint64_t target = divs[i];

        /* binary search in diffs */
        size_t lo = 0, hi = diffcnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            uint64_t val = diffs[mid];
            if (val == target) { ++count; break; }
            else if (val < target) lo = mid + 1;
            else hi = mid;
        }
    }

    return (seq_t)count;
}
