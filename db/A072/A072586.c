/*
 * A072586  Number of numbers <= n having prime factors with odd exponents
 *          only.
 * https://oeis.org/A072586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers ≤ n whose prime factorisations have
 *            all exponents odd
 * Range:     u8 n <= 361, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* smallest‑prime‑factor sieve */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    size_t new_size = (size_t)(limit + 1);
    idx_t *tmp = (idx_t *)realloc(spf, new_size * sizeof(idx_t));
    if (!tmp) exit(1);
    spf = tmp;
    for (size_t i = 0; i < new_size; ++i) spf[i] = (idx_t)i;
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (spf[p] == p) {
            for (idx_t j = p * p; j <= limit; j += p) {
                if (spf[j] == j) spf[j] = p;
            }
        }
    }
    spf_limit = limit;
}

/* cumulative count array */
static idx_t max_computed = 0;
static seq_t *arr = NULL;
static size_t arr_capacity = 0;

static void ensure_arr(idx_t limit)
{
    if ((size_t)limit < arr_capacity) return;
    size_t new_cap = (size_t)(limit + 1);
    seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    arr = tmp;
    for (size_t i = arr_capacity; i < new_cap; ++i) arr[i] = (seq_t)0;
    arr_capacity = new_cap;
}

static int all_exponents_odd(idx_t x)
{
    idx_t cur = x;
    while (cur > 1) {
        idx_t p = spf[cur];
        int e = 0;
        while (cur % p == 0) {
            ++e;
            cur /= p;
        }
        if ((e & 1) == 0) return 0; /* even exponent found */
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_spf(n);
    ensure_arr(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            arr[1] = (seq_t)0;            /* 1 has no prime factors */
        } else {
            arr[i] = arr[i - 1] + (seq_t)(all_exponents_odd(i) ? 1 : 0);
        }
    }
    max_computed = n;
    return arr[n];
}
