/*
 * A140222  A number j is included if (Sum_{k=1..j} d(k)) is prime, where d(k)
 *          is the number of divisors of k.
 * https://oeis.org/A140222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A140222, the sequence of integers j such that
 * the cumulative sum Σ_{k=1..j} d(k) (where d(k) is the number of
 * divisors of k) is prime.  The function returns the n‑th term
 * (a(1) = 2).  It recomputes the sequence up to the required
 * length on each call; the algorithm runs in O(n) time for
 * input n and uses only standard C headers. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Simple divisor count: number of positive divisors of n. */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t cnt = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (tmp > 1) {
        cnt *= 2;
    }
    return cnt;
}

/* Integer primality test (trial division) suitable for the small
 * values that occur as cumulative sums in this sequence. */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Allocate a temporary array to hold the first n terms. */
    seq_t *terms = (seq_t *)malloc(n * sizeof(seq_t));
    if (!terms) return (seq_t)0;  /* handle allocation failure */

    idx_t   count = 0;      /* number of terms stored so far   */
    idx_t   j     = 1;      /* current index being tested      */
    seq_t   sum   = 0;      /* cumulative sum of divisor counts */

    while (count < n) {
        uint64_t d = divisor_count(j);
        sum += (seq_t)d;

        if (is_prime((uint64_t)sum)) {
            terms[count++] = (seq_t)j;
        }
        ++j;
    }

    seq_t result = terms[n - 1];
    free(terms);
    return result;
}
