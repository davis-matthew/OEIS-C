/*
 * A129294  Number of divisors of n^3 - 1 that are not greater than n.
 * https://oeis.org/A129294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d : 1 <= d <= n and (n^3 – 1) mod d = 0}
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1905, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build a list of primes up to 1,000,000 */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    const size_t limit = 1000000; /* sqrt(10^12) */
    bool *is_composite = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_composite) return; /* allocation failed */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }
    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_composite);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            primes[idx++] = (uint32_t)i;

    free(is_composite);
    primes_built = true;
}

/* Recursive enumeration of divisors */
static void enumerate_divisors(int pos,
                               seq_t prod,
                               const seq_t *pf,
                               const int *exp,
                               int pf_cnt,
                               idx_t n,
                               seq_t *cnt)
{
    if (pos == pf_cnt) {
        if (prod <= (seq_t)n) ++(*cnt);
        return;
    }

    seq_t cur = prod;
    for (int e = 0; e <= exp[pos]; ++e) {
        if (cur > (seq_t)n) return;
        enumerate_divisors(pos + 1, cur, pf, exp, pf_cnt, n, cnt);
        cur *= pf[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1; /* 0 has infinite divisors; only 1 qualifies */

    /* Compute m = n^3 - 1 */
    seq_t m = (seq_t)n;
    m *= (seq_t)n;
    m *= (seq_t)n;
    m -= (seq_t)1;

    build_primes();

    /* Factorize m */
    seq_t pf[16];
    int exp[16];
    int pf_cnt = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        seq_t pseq = (seq_t)p;
        if (pseq * pseq > m) break;
        int e = 0;
        while (m % pseq == 0) {
            m /= pseq;
            ++e;
        }
        if (e > 0) {
            pf[pf_cnt] = pseq;
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (m > 1) {
        /* Remaining prime factor */
        pf[pf_cnt] = m;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    seq_t count = 0;
    enumerate_divisors(0, (seq_t)1, pf, exp, pf_cnt, n, &count);
    return count;
}
