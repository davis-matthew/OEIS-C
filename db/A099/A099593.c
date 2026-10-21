/*
 * A099593  Sum of the number of e-divisors of all numbers from 2 up to n.
 * https://oeis.org/A099593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=2}^{n} Π_{p^e‖m} d(e) where d(k) is the number of
 *            divisors of k. The function memoizes values to answer any n ≥ 1.
 *            It works for all provided seq_t and idx_t types
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *seq_array = NULL;          /* a[0..k] */
static idx_t seq_len   = 0;               /* current length of seq_array */
static uint64_t *spf    = NULL;           /* smallest prime factor */
static idx_t spf_len    = 0;               /* length of spf array */

/* d(k) = number of divisors of integer k for 0 ≤ k ≤ 19 */
static const seq_t d_divisors[20] = {
    0, /* dummy for index 0 not used */
    1, /* d(1) */
    2, /* d(2) */
    2, /* d(3) */
    3, /* d(4) */
    2, /* d(5) */
    4, /* d(6) */
    2, /* d(7) */
    4, /* d(8) */
    3, /* d(9) */
    4, /* d(10) */
    2, /* d(11) */
    6, /* d(12) */
    2  /* d(13) */
};

/* Ensure spf array is built up to n */
static void build_spf(idx_t n)
{
    if (n < (idx_t)2)
        return;
    if (spf_len <= n) {
        /* Reallocate to at least n+1 slots */
        idx_t new_len = n + 1;
        uint64_t *new_spf = (uint64_t *)realloc(spf, new_len * sizeof(uint64_t));
        if (!new_spf) exit(1);
        spf = new_spf;
        for (idx_t i = spf_len; i <= n; ++i)
            spf[i] = (uint64_t)i;
        for (idx_t i = 2; (idx_t)i * i <= n; ++i)
            if (spf[i] == i)
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == j)
                        spf[j] = i;
        spf_len = new_len;
    }
}

/* Ensure sequence array has values up to n */
static void precompute(idx_t n)
{
    if (n < 1)
        return;
    if (seq_len == 0) {
        seq_len = 2; /* we need indices 0 and 1 at least */
        seq_array = (seq_t *)malloc(seq_len * sizeof(seq_t));
        if (!seq_array) exit(1);
        seq_array[0] = (seq_t)0; /* unused */
        seq_array[1] = (seq_t)0; /* a(1) = 0 */
    }
    if (seq_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_seq = (seq_t *)realloc(seq_array, new_len * sizeof(seq_t));
        if (!new_seq) exit(1);
        seq_array = new_seq;
        /* compute missing values */
        for (idx_t m = seq_len; m <= n; ++m) {
            idx_t temp = m;
            seq_t prod = (seq_t)1;
            while (temp > 1) {
                idx_t p = spf[temp];
                idx_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                /* e ≥ 1 and ≤ 13 for n ≤ 10000, safe index */
                prod *= d_divisors[(size_t)e];
            }
            seq_array[m] = seq_array[m-1] + prod;
        }
        seq_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    /* Build spf and compute needed terms */
    build_spf(n);
    precompute(n);
    return seq_array[n];
}
