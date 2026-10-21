/*
 * A071364  Smallest number with same sequence of exponents in canonical prime
 *          factorization as n.
 * https://oeis.org/A071364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 7952, u32 n <= 7952, u64 n <= 7952,
 *            u128 n <= 7952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071364: Smallest number with the same exponent sequence in the
   canonical prime factorization as n.
   a(n) = ∏_{k=1}^{ω(n)} p_k^{e_k}
   where p_k is the k‑th prime (2,3,5,…) and e_k is the exponent of the
   k‑th smallest prime factor of n.
*/

#include <stdint.h>

/* First 20 primes – enough for any distinct prime factor count that
   can appear in a 64‑bit integer (maximum 16 distinct primes). */
static const uint64_t first_primes[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};

seq_t a(idx_t n)
{
    /* n is 1‑based index as per OEIS specification. */
    if (n == 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;          /* work in uint64_t for factoring */
    int exps[20] = {0};                   /* exponent list, up to 20 slots */
    int cnt = 0;                          /* number of distinct primes */

    /* Count factor 2 separately */
    if (temp % 2ULL == 0) {
        int e = 0;
        while (temp % 2ULL == 0) {
            temp /= 2ULL;
            ++e;
        }
        exps[cnt++] = e;
    }

    /* Odd factors */
    for (uint64_t p = 3ULL; p * p <= temp; p += 2ULL) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }

    if (temp > 1ULL) { /* remaining prime > sqrt(original) */
        exps[cnt++] = 1;
    }

    /* Compute the result using the first cnt primes */
    seq_t result = (seq_t)1;
    for (int i = 0; i < cnt; ++i) {
        seq_t prime = (seq_t)first_primes[i];
        seq_t power = (seq_t)1;
        int e = exps[i];
        /* simple exponentiation; e <= 64 for uint64_t n */
        while (e) {
            if (e & 1) power *= prime;
            prime *= prime;
            e >>= 1;
        }
        result *= power;
    }
    return result;
}
