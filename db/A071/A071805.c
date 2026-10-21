/*
 * A071805  Number of pairs (x,y) such that prime(x) + prime(y) = x*tau(x) +
 *          y*tau(y), 1<=x<=y<=n.
 * https://oeis.org/A071805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y), 1 ≤ x ≤ y ≤ n, such that *
 *            prime(x) + prime(y) = x·τ(x) + y·τ(y) where τ(k) is the divisor
 *            cardinality of k
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   divisor_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_counts.h"

/* Simple sieve that fills *primes with the primes ≤ limit.
 * The caller must free (*primes).  *cnt counts them. */
static void sieve_primes(idx_t limit, uint64_t **primes, size_t *cnt)
{
    bool *mark = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!mark) { *primes = NULL; *cnt = 0; return; }

    for (idx_t i = 0; i <= limit; ++i) mark[i] = false;
    for (idx_t i = 2; i <= limit; ++i) mark[i] = true;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (mark[p]) {
            for (idx_t j = p * p; j <= limit; j += p) mark[j] = false;
        }
    }

    size_t num = 0;
    for (idx_t i = 2; i <= limit; ++i) if (mark[i]) ++num;

    uint64_t *arr = (uint64_t*)malloc(num * sizeof(uint64_t));
    if (!arr) { free(mark); *primes = NULL; *cnt = 0; return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (mark[i]) arr[pos++] = i;

    free(mark);
    *primes = arr;
    *cnt = num;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Step 1: generate at least n primes */
    uint64_t *prime_arr = NULL;
    size_t prime_cnt = 0;
    idx_t sieve_limit = n * 20 + 100;               /* safe bound for n ≤ 10000 */
    do {
        sieve_primes(sieve_limit, &prime_arr, &prime_cnt);
        if (prime_cnt >= n) break;
        free(prime_arr);
        sieve_limit *= 2;
    } while (1);

    /* Step 2: divisor counts */
    uint16_t *tau_arr = NULL;
    divisor_counts(n, &tau_arr);
    if (!tau_arr) { free(prime_arr); return (seq_t)0; }

    /* Step 3: f(x) = prime(x) – x·τ(x) */
    int64_t *f_vals = (int64_t*)malloc((size_t)n * sizeof(int64_t));
    if (!f_vals) { free(prime_arr); free(tau_arr); return (seq_t)0; }

    int64_t min_f = 0, max_f = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int64_t p  = (int64_t)prime_arr[i - 1];
        int64_t t  = (int64_t)tau_arr[i];
        int64_t f  = p - (int64_t)i * t;
        f_vals[i - 1] = f;
        if (i == 1) { min_f = max_f = f; }
        else {
            if (f < min_f) min_f = f;
            if (f > max_f) max_f = f;
        }
    }

    /* Step 4: count pairs using a frequency table over shifted f-values */
    size_t diff = (size_t)(max_f - min_f + 1);
    seq_t *cnt_arr = (seq_t*)calloc(diff, sizeof(seq_t));
    seq_t ans = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        int64_t f   = f_vals[i - 1];
        size_t idx  = (size_t)(f - min_f);          /* index of f in table */

        if (f == 0) ans += (seq_t)1;                /* pair (i,i) */

        int64_t neg = -f;
        if (neg >= min_f && neg <= max_f) {
            size_t tgt = (size_t)(neg - min_f);
            ans += cnt_arr[tgt];
        }
        cnt_arr[idx] += (seq_t)1;
    }

    /* Clean up */
    free(f_vals);
    free(cnt_arr);
    free(prime_arr);
    free(tau_arr);

    return ans;
}
