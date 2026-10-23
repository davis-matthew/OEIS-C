/*
 * A172235  n-th partial sum of primes of the form 3*k+1/2+-1/2 minus n-th
 *          partial sum of primes of the form 3*m-1.
 * https://oeis.org/A172235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A172113(n) – A172188(n) A172113(n) : sum of the first n
 *            primes of the form 3k+1 (including 3) A172188(n) : sum of the
 *            first n primes of the form 3k-1 (i.e. ≡2 (mod 3))
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build */
seq_t a(idx_t n)
{
    /* static state for memoization and on‑the‑fly prime generation */
    static bool initialized = false;

    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0, prime_cnt = 0;

    static seq_t *sum1 = NULL, *sum2 = NULL;
    static size_t sum1_cap = 0, sum2_cap = 0;
    static size_t count1 = 0, count2 = 0;

    static idx_t next_candidate = 2;          /* next integer to test for primality */

    /* one‑time allocation and initialization */
    if (!initialized) {
        /* initial capacities (can be grown later) */
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(sizeof(uint64_t) * prime_cap);

        sum1_cap = 4;
        sum1 = (seq_t *)malloc(sizeof(seq_t) * sum1_cap);
        sum1[0] = 0;                          /* sentinel for sums */

        sum2_cap = 4;
        sum2 = (seq_t *)malloc(sizeof(seq_t) * sum2_cap);
        sum2[0] = 0;                          /* sentinel */

        initialized = true;
    }

    size_t target = (size_t)n;                /* n is guaranteed ≥ 1 */

    /* generate additional primes until we have ≥ n primes in each class */
    while (count1 < target || count2 < target) {
        uint64_t cand = (uint64_t)next_candidate;
        next_candidate++;                     /* advance to the next candidate */

        /* skip even numbers greater than 2 */
        if (cand > 2 && (cand & 1ULL) == 0)
            continue;

        /* trial division primality test */
        bool isprime = true;
        if (cand < 2)
            isprime = false;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                isprime = false;
                break;
            }
        }

        if (!isprime)
            continue;

        /* store the new prime */
        if (prime_cnt == prime_cap) {
            prime_cap <<= 1;
            prime_list = (uint64_t *)realloc(prime_list, sizeof(uint64_t) * prime_cap);
        }
        prime_list[prime_cnt++] = cand;

        /* classify the prime and update the corresponding partial sums */
        if (cand == 3 || cand % 3 == 1) {                /* class 1: 3 or ≡1 (mod 3) */
            if (sum1_cap <= count1 + 1) {
                sum1_cap <<= 1;
                sum1 = (seq_t *)realloc(sum1, sizeof(seq_t) * sum1_cap);
            }
            sum1[count1 + 1] = sum1[count1] + (seq_t)cand;
            ++count1;
        } else if (cand % 3 == 2) {                     /* class 2: ≡2 (mod 3) */
            if (sum2_cap <= count2 + 1) {
                sum2_cap <<= 1;
                sum2 = (seq_t *)realloc(sum2, sizeof(seq_t) * sum2_cap);
            }
            sum2[count2 + 1] = sum2[count2] + (seq_t)cand;
            ++count2;
        }
    }

    /* return the difference of the n‑th partial sums */
    return sum1[target] - sum2[target];
}
