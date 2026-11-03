/*
 * A069161  Numbers n such that no group of order n can be a central factor.
 * https://oeis.org/A069161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069161 (OEIS):                 */
/* Numbers n such that no group of order n   */
/* can be a central factor.                 */
/* Computation: scan odd integers, skip 2   */
/* except for a(1), and include only those */
/* that are not (a) odd prime powers with   */
/* exponent > 1, nor (b) having two distinct*/
/* prime divisors p<q with p|(q-1).         */
/* a(1) = 2.                                */

#include <stdint.h>
#include <stdbool.h>

/* Test whether an odd integer x should be included
   in the sequence. Returns true if included. */
static bool include_odd(uint64_t x)
{
    if (x == 1)
        return false;           /* excluded */

    uint64_t temp = x;
    unsigned long long primes[16];
    int exps[16];
    int pc = 0;                 /* number of distinct primes */

    /* factorization by simple trial division */
    for (unsigned long long p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            do {
                temp /= p;
                ++e;
            } while (temp % p == 0);
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (temp > 1) {             /* remaining prime factor */
        primes[pc] = temp;
        exps[pc]   = 1;
        ++pc;
    }

    /* Exclude odd prime powers with exponent > 1 */
    if (pc == 1 && exps[0] > 1)
        return false;

    /* Exclude numbers that have a pair of distinct
       prime divisors p < q with p | (q-1) */
    for (int i = 0; i < pc; ++i) {
        for (int j = i + 1; j < pc; ++j) {
            unsigned long long p = primes[i];
            unsigned long long q = primes[j];
            if (p < q) {
                if ((q - 1) % p == 0)
                    return false;
            } else {          /* q < p */
                if ((p - 1) % q == 0)
                    return false;
            }
        }
    }

    return true;                /* included */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0 */

    /* a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    idx_t count = 1;            /* we already have the term 2 */
    uint64_t candidate = 3;     /* start checking from the first odd integer ≥3 */

    while (count < n) {
        if (include_odd(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;         /* only odd numbers */
    }

    return (seq_t)0;            /* should never reach here for valid n */
}
