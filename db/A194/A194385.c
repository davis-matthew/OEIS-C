/*
 * A194385  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(10) and < > denotes fractional part.
 * https://oeis.org/A194385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A194385:
 * numbers m such that Sum_{k=1..m} (<1/2 + k*sqrt(10)> - <k*sqrt(10)>) = 0.
 * Here < > denotes fractional part.  The sum being zero
 * is equivalent to the number of terms with fractional part < 1/2
 * up to m equals the number of terms with fractional part ≥ 1/2. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Helper: compute floor(k*sqrt(10)) via integer square root:
 * floor(sqrt(10*k^2)) uses only integer arithmetic. */
static inline seq_t floor_k_sqrt10(idx_t k)
{
    __uint128_t tmp = (__uint128_t)10 * k * k;
    return floor_sqrt((seq_t)tmp);
}

/* Helper: compute floor(2k*sqrt(10)) via integer square root:
 * floor(sqrt(40*k^2)) uses only integer arithmetic. */
static inline seq_t floor_2k_sqrt10(idx_t k)
{
    __uint128_t tmp = (__uint128_t)40 * k * k;
    return floor_sqrt((seq_t)tmp);
}

/* Main routine: return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safeguard */

    int sum = 0;          /* current cumulative sum of ±1 */
    idx_t k = 1;          /* current k value */
    idx_t hits = 0;       /* number of times cumulative sum has become zero */

    for (;; ++k) {
        seq_t t = floor_k_sqrt10(k);                /* floor(k*sqrt(10)) */
        seq_t u = floor_2k_sqrt10(k);               /* floor(2k*sqrt(10)) */

        /* u - 2*t is 0 when fractional part of k*sqrt(10) < 0.5,
         * otherwise 1.  This determines the step in the partial sum. */
        int diff = (int)((u - (t << 1)) == 0 ? 0 : 1);
        if (diff == 0) sum += 1; else sum -= 1;

        if (sum == 0) {
            ++hits;
            if (hits == n) {
                return (seq_t)k;
            }
        }
    }
}
