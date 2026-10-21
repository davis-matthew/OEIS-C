/*
 * A075494  Squares whose sum of digits exceeds the number of divisors.
 * https://oeis.org/A075494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A075494 – Squares whose sum of digits exceeds the number of divisors
    a(n) = the n-th square k² with digitSum(k²) > τ(k²)      (offset 1)

    The function finds the n‑th such square by scanning k = 1,2,3,…,
    computing k², its digit sum, factorising k to obtain τ(k²)
    = ∏(2·e_i + 1), and testing the inequality.  A small sieve
    precomputes the primes up to a fixed limit (200 000) used for the
    trial division in the factorisation.  This approach runs in
    O(n log n) time and easily meets the 2‑second requirement for
    n ≤ 10 000.  All arithmetic uses only integer operations.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool primes_inited = false;
static int  *prime_list = NULL;
static size_t prime_count = 0;

/* Simple sieve of Eratosthenes – generates all primes ≤ limit.  */
static void init_primes(unsigned limit)
{
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    memset(is_comp, 0, (limit + 1) * sizeof(bool));

    /* Allocate maximum possible space for primes – comfortably enough. */
    prime_list = (int*)malloc((limit + 1) * sizeof(int));
    prime_count = 0;

    for (unsigned i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_count++] = (int)i;
            if ((uint64_t)i * i <= limit) {
                for (unsigned j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_inited = true;
}

/* Compute τ(k²) = ∏ (2·e_i + 1), where k = ∏ p_i^{e_i}.  */
static seq_t divisor_count_square(uint64_t k)
{
    seq_t res = 1;
    uint64_t temp = k;

    for (size_t idx = 0; idx < prime_count && (uint64_t)prime_list[idx] * prime_list[idx] <= temp; ++idx) {
        int p = prime_list[idx];
        int e = 0;
        while (temp % (uint64_t)p == 0) {
            temp /= (uint64_t)p;
            ++e;
        }
        if (e > 0)
            res *= (seq_t)(2 * e + 1);
    }
    if (temp > 1)           /* remaining prime factor of exponent 1 */
        res *= 3;           /* 2*1 + 1 = 3 */
    return res;
}

/* Sum of decimal digits of s (s fits in seq_t).  */
static unsigned digit_sum_seq(seq_t s)
{
    unsigned sum = 0;
    while (s > 0) {
        sum += (unsigned)(s % 10);
        s /= 10;
    }
    return sum;
}

/* Main sequence generator.  */
seq_t a(idx_t n)
{
    if (!primes_inited)
        init_primes(200000);          /* safe upper bound for n ≤ 10 000 */

    idx_t count = 0;
    idx_t k = 1;

    while (true) {
        seq_t square = (seq_t)k * (seq_t)k;

        /* Test condition */
        if (digit_sum_seq(square) > divisor_count_square((uint64_t)k)) {
            ++count;
            if (count == n)
                return square;
        }
        ++k;
    }
}
