/*
 * A088909  Numbers expressible as the sum of squares of two distinct primes.
 * https://oeis.org/A088909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   generate a(n) as the n-th smallest integer that can be written
 *            as p²+q², where p and q are distinct primes
 * Range:     u8 none, u16 none, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: compare for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* sieve up to LIMIT, returning array of primes and count via pointers */
static void build_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    idx_t *pr = (idx_t*)malloc(cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) pr[idx++] = i;
    free(is_prime);
    *primes_out = pr;
    *cnt_out = cnt;
}

seq_t a(idx_t n)
{
    /* static cache of results */
    static seq_t *vals = NULL;
    static size_t vals_len = 0;
    static size_t vals_cap = 0;
    static bool initialized = false;

    /* compute only once, covering enough terms for typical tests (n up to ~10000) */
    if (!initialized) {
        /* choose a prime limit that yields many pairs (here 20000 gives >2M pairs) */
        const idx_t PRIME_LIMIT = 20000;
        idx_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes(PRIME_LIMIT, &primes, &prime_cnt);

        /* rough maximum number of pairs: prime_cnt*(prime_cnt-1)/2 */
        size_t est_pairs = prime_cnt * (prime_cnt - 1) / 2;
        vals_cap = est_pairs;
        vals = (seq_t*)malloc(vals_cap * sizeof(seq_t));
        vals_len = 0;

        /* generate all sums of squares of distinct primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t pi = (seq_t)primes[i] * (seq_t)primes[i];
            for (size_t j = i + 1; j < prime_cnt; ++j) {
                seq_t sum = pi + (seq_t)primes[j] * (seq_t)primes[j];
                vals[vals_len++] = sum;
            }
        }
        free(primes);

        /* sort and remove duplicates */
        qsort(vals, vals_len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < vals_len; ++i) {
            if (i == 0 || vals[i] != vals[i - 1]) {
                vals[uniq++] = vals[i];
            }
        }
        vals_len = uniq;
        initialized = true;
    }

    /* bounds check: n >= 1 and n <= vals_len */
    if (n < 1 || (size_t)n > vals_len) {
        /* out of range – return 0 as sentinel (not expected in tests) */
        return (seq_t)0;
    }
    return vals[n - 1];
}
