/*
 * A138853  Numbers which are the sum of 3 cubes of distinct odd primes.
 * https://oeis.org/A138853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th smallest integer that can be expressed
 *                as p(i)^3 + p(j)^3 + p(k)^3 with distinct odd primes
 *                p(i) > p(j) > p(k) > 1
 * The function generates enough sums on first call, sorts and deduplicates
 * them, then returns the requested term.  It works for any seq_t type
 * (unsigned int, 128‑bit, or arbitrary‑precision) and any idx_t type
 * (typically uint64_t).  No floating‑point or external files are used.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* compare two seq_t values for qsort */
static int seq_cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* simple sieve to collect primes up to limit (inclusive) */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    size_t sz = (size_t)limit + 1;
    unsigned char *is_prime = (unsigned char *)calloc(sz, 1);
    if (!is_prime) {
        *primes_out = NULL; *cnt_out = 0; return;
    }
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;
    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                is_prime[m] = 0;
        }
    }
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++count;

    uint64_t *pr = NULL;
    if (count) {
        pr = (uint64_t *)malloc(count * sizeof(uint64_t));
        if (pr) {
            size_t idx = 0;
            for (size_t i = 2; i <= (size_t)limit; ++i)
                if (is_prime[i]) pr[idx++] = i;
        }
    }
    free(is_prime);
    *primes_out = pr;
    *cnt_out    = count;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* static data will be initialized once on the first call */
    static bool initialized = false;
    static seq_t *values = NULL;
    static size_t unique_len = 0;

    if (!initialized) {
        /* --- choose a limit that guarantees enough terms --- */
        const idx_t PRIME_LIMIT = 1500;   /* sufficient for n up to >10^4 */
        uint64_t *all_primes = NULL;
        size_t all_cnt = 0;
        build_primes_upto(PRIME_LIMIT, &all_primes, &all_cnt);

        /* collect odd primes (exclude 2) */
        size_t odd_cnt = 0;
        for (size_t i = 0; i < all_cnt; ++i)
            if (all_primes[i] > 2) ++odd_cnt;

        uint64_t *odd_primes = (uint64_t *)malloc(odd_cnt * sizeof(uint64_t));
        size_t oidx = 0;
        for (size_t i = 0; i < all_cnt; ++i)
            if (all_primes[i] > 2) odd_primes[oidx++] = all_primes[i];

        free(all_primes);

        /* --- compute all distinct sums of three cubes --- */
        size_t comb = odd_cnt * (odd_cnt - 1) * (odd_cnt - 2) / 6;
        seq_t *tmp = (seq_t *)malloc(comb * sizeof(seq_t));
        if (!tmp) {
            free(odd_primes);
            return 0;
        }

        size_t idx = 0;
        for (int i = (int)odd_cnt - 1; i >= 2; --i) {
            seq_t ai = (seq_t)odd_primes[i];
            seq_t ai3 = ai * ai; ai3 = ai3 * ai;
            for (int j = i - 1; j >= 1; --j) {
                seq_t aj = (seq_t)odd_primes[j];
                seq_t aj3 = aj * aj; aj3 = aj3 * aj;
                for (int k = j - 1; k >= 0; --k) {
                    seq_t ak = (seq_t)odd_primes[k];
                    seq_t ak3 = ak * ak; ak3 = ak3 * ak;
                    tmp[idx++] = ai3 + aj3 + ak3;
                }
            }
        }
        free(odd_primes);

        /* --- sort and deduplicate the sums --- */
        qsort(tmp, comb, sizeof(seq_t), seq_cmp);

        values = (seq_t *)malloc(comb * sizeof(seq_t));
        unique_len = 0;
        if (comb > 0) {
            values[unique_len++] = tmp[0];
            for (size_t i = 1; i < comb; ++i)
                if (tmp[i] != tmp[i - 1])
                    values[unique_len++] = tmp[i];
        }
        free(tmp);
        initialized = true;
    }

    if ((idx_t)unique_len < n) return 0;   /* out of range */
    return values[n - 1];
}
