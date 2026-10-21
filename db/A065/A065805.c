/*
 * A065805  a(n) = Sum_{j=0..n} sigma_j(n).
 * https://oeis.org/A065805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d^(n+1)-1)/(d-1), with d=1 giving the term n+1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, enumerates all its divisors, and for each divisor
   >1 computes (d^(n+1)-1)/(d-1) using the helper big_pow, adding the contributions
   to the running sum. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle potential n==0 gracefully; though offset is 1 */
    if (n == 0) return 0;

    seq_t sum = (seq_t)n + (seq_t)1;   /* divisor d = 1 contributes n+1 */

    /* ---- Factor n ---- */
    idx_t temp = n;
    idx_t primes[64];
    int exps[64];
    size_t pcnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt]   = temp;
        exps[pcnt]     = 1;
        ++pcnt;
    }

    /* ---- Enumerate all divisors ---- */
    uint64_t divisors[2048];
    size_t dcnt = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t base   = (uint64_t)primes[i];
        int     exp     = exps[i];
        size_t  curCnt  = dcnt;
        uint64_t mult   = 1;

        for (int e = 1; e <= exp; ++e) {
            mult *= base;
            for (size_t j = 0; j < curCnt; ++j) {
                divisors[dcnt++] = divisors[j] * mult;
            }
        }
    }

    /* ---- Accumulate contributions from each divisor d>1 ---- */
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        if (d == 1) continue;

        seq_t d_seq = (seq_t)d;
        seq_t pow   = big_pow(d_seq, (seq_t)(n + 1));          /* d^(n+1) */
        seq_t term  = (pow - (seq_t)1) / ((seq_t)d - (seq_t)1);  /* (d^(n+1)-1)/(d-1) */
        sum += term;
    }

    return sum;
}
