/*
 * A209312  Number of practical numbers p<n with n-p and n+p both prime or
 *          both practical.
 * https://oeis.org/A209312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of practical numbers p < n such that * (n-p) and
 *            (n+p) are both prime or both practical
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9788 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime_seq.h"

/* Check whether a number is practical.
 * Implements the known characterization:
 *   For the prime factorization n = ∏ p_i^a_i with sorted primes,
 *   let σ_i be the sum of divisors of the product ∏_{j<i} p_j^a_j.
 *   Then n is practical iff p_1=2 and for all i≥2, p_i ≤ σ_{i-1} + 1.   */
static bool isPractical(uint64_t n)
{
    if (n == 1) return true;
    if ((n & 1ULL) != 0ULL) return false;   /* all practical >1 are even */

    uint64_t rem = n;
    uint64_t exp2 = 0;
    while ((rem & 1ULL) == 0ULL) { rem >>= 1; ++exp2; }

    /* sum of divisors of the processed part (σ(previous)) */
    uint64_t sigma = ((uint64_t)1 << (exp2 + 1)) - 1;  /* 2^(exp2+1)-1 */

    /* process remaining prime factors in ascending order */
    uint64_t p = 3;
    while (p * p <= rem) {
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) { rem /= p; ++e; }

            if (p > sigma + 1) return false;

            /* factor = (p^(e+1)-1)/(p-1)  */
            __int128 pow = 1;
            for (uint64_t i = 0; i < e + 1; ++i) pow *= p;
            uint64_t factor = (uint64_t)((pow - 1) / (p - 1));
            sigma *= factor;
        }
        p += 2;
    }

    if (rem > 1) {      /* remaining prime factor > sqrt(original) */
        uint64_t p = rem;   /* exponent 1 */
        if (p > sigma + 1) return false;
        __int128 pow = (__int128)p * p;   /* p^(1+1) */
        uint64_t factor = (uint64_t)((pow - 1) / (p - 1));
        sigma *= factor;
    }

    return true;
}

/* Static cache of practical numbers */
static uint64_t *practical_list = NULL;   /* array of practical numbers */
static size_t practical_cnt   = 0;        /* number of elements in array   */
static size_t practical_cap   = 0;        /* allocated capacity             */
static size_t practical_max   = 0;        /* max number for which we have
                                           a practicality test               */

/* Ensure practical numbers up to limit-1 are generated and stored */
static void ensure_practicals_up_to(size_t limit)
{
    if (limit <= practical_max) return;   /* already done */

    for (size_t num = practical_max + 1; num <= limit; ++num) {
        if (isPractical(num)) {
            if (practical_cnt == practical_cap) {
                size_t new_cap = practical_cap ? (practical_cap << 1) : 16;
                uint64_t *new_arr = (uint64_t *)realloc(practical_list,
                                                        new_cap * sizeof(uint64_t));
                assert(new_arr);
                practical_list = new_arr;
                practical_cap = new_cap;
            }
            practical_list[practical_cnt++] = num;
        }
    }
    practical_max = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(1)=0, a(2)=0 from the sequence definition */

    /* Build practical numbers up to n-1 once */
    ensure_practicals_up_to((size_t)n - 1);

    seq_t count = 0;

    for (size_t i = 0; i < practical_cnt; ++i) {
        uint64_t p = practical_list[i];
        if (p >= n) break;                  /* safety in case of mis‑run */

        uint64_t m1 = n - p;
        uint64_t m2 = n + p;

        bool both_prime = is_prime_seq(m1) && is_prime_seq(m2);
        bool both_practical = isPractical(m1) && isPractical(m2);

        if (both_prime || both_practical) ++count;
    }
    return count;
}
