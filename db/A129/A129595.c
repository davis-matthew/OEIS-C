/*
 * A129595  Array A(i,j): A(1,1), A(2,1), A(1,2), A(3,1), A(2,2), A(1,3), ...
 *          of elementwise sums of partitions encoded in the prime
 *          factorizations of i and j.
 * https://oeis.org/A129595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 283, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 88 terms through n = 1214 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129595: a(n) = integer obtained by element‑wise adding the partitions
 * encoded by the two indices of the n‑th anti‑diagonal of the array, then
 * converting the resulting partition back to an integer.  Indices start at 1.
 */
#include <stdint.h>
#include <math.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

/* ----------------------------------------------------------------------
 * Add two partitions element‑wise, starting from the largest parts.
 * ---------------------------------------------------------------------- */
static void add_partitions(const int A[], int La,
                           const int B[], int Lb,
                           int res[], int *reslen)
{
    int maxlen = La > Lb ? La : Lb;
    if (maxlen == 0) { *reslen = 0; return; }

    int temp[maxlen];
    for (int k = 0; k < maxlen; ++k) {
        int a = (La - 1 - k >= 0) ? A[La - 1 - k] : 0;
        int b = (Lb - 1 - k >= 0) ? B[Lb - 1 - k] : 0;
        temp[k] = a + b;                /* from largest to smallest */
    }
    /* reverse to ascending order */
    for (int k = 0; k < maxlen; ++k)
        res[k] = temp[maxlen - 1 - k];
    *reslen = maxlen;
}

/* ----------------------------------------------------------------------
 * Map the n-th term (1‑based) to the pair (i,j) on the anti‑diagonal.
 * ---------------------------------------------------------------------- */
static void anticlique_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Solve t such that t(t+1)/2 >= n.  Use double to get a rough estimate,
       then adjust. */
    double t0 = (sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0;
    idx_t t = (idx_t)t0;
    /* Correct any mis‑rounding */
    while ((__uint128_t)t * (t + 1) / 2 < n) ++t;
    while (t > 0 && (__uint128_t)(t - 1) * t / 2 >= n) --t;

    idx_t prev = (__uint128_t)(t - 1) * t / 2;   /* T_{t-1} */
    idx_t r     = n - prev;                      /* 1‑based offset on this diagonal */
    *i = t - r + 1;                              /* i decreases from t to 1 */
    *j = r;                                      /* j increases from 1 to t */
}

/* ----------------------------------------------------------------------
 * Convert exponent tuple to the ascending integer partition as defined in
 * the OEIS description.
 * ---------------------------------------------------------------------- */
static void exps_to_partition(const int exps[], int len,
                              int parts[], int *partslen)
{
    if (len == 0) { *partslen = 0; return; }
    if (len == 1) {
        parts[0] = exps[0];
        *partslen = 1;
        return;
    }
    int L = len;
    parts[0] = exps[0] + 1;
    for (int i = 1; i < L - 1; ++i)
        parts[i] = parts[i-1] + exps[i];
    parts[L-1] = parts[L-2] + exps[L-1] - 1;
    *partslen = L;
}

/* primes up to 199 – plenty for indices <= 10 000 (max i or j <= 141). */
static const int PRIMES[46] = {
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199
};
static const int NUM_PRIMES = 46;

/* ----------------------------------------------------------------------
 * Factor n into prime exponents.  exps[] must have space for NUM_PRIMES.
 * ---------------------------------------------------------------------- */
static void factor_exponents(idx_t n, int exps[], int *len)
{
    if (n <= 1) { *len = 0; return; }
    idx_t m = n;
    int idx = 0;
    for (int pi = 0; pi < NUM_PRIMES && m > 1; ++pi)
    {
        int p   = PRIMES[pi];
        int cnt = 0;
        while (m % p == 0) { ++cnt; m /= p; }
        exps[idx++] = cnt;
        if (m == 1) break;
    }
    *len = idx;
}

/* ----------------------------------------------------------------------
 * Convert a partition back to the integer encoded by that partition.
 * ---------------------------------------------------------------------- */
static seq_t partition_to_int(const int parts[], int len)
{
    if (len == 0) return seq_one();

    int exps[NUM_PRIMES];
    int k;
    if (len == 1) {
        exps[0] = parts[0];
        k = 1;
    } else {
        exps[0] = parts[0] - 1;
        for (int i = 1; i < len - 1; ++i)
            exps[i] = parts[i] - parts[i-1];
        exps[len-1] = parts[len-1] - parts[len-2] + 1;
        k = len;
    }

    seq_t res = seq_one();
    for (int i = 0; i < k; ++i) {
        seq_t p = ipow((seq_t)PRIMES[i], (seq_t)exps[i]);
        res = seq_mul(res, p);
    }
    return res;
}

/* ----------------------------------------------------------------------
 * Main sequence function.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t i, j;
    anticlique_coords(n, &i, &j);

    int exps_i[NUM_PRIMES], exps_j[NUM_PRIMES];
    int len_i, len_j;
    factor_exponents(i, exps_i, &len_i);
    factor_exponents(j, exps_j, &len_j);

    int parts_i[NUM_PRIMES], parts_j[NUM_PRIMES];
    int p_len_i, p_len_j;
    exps_to_partition(exps_i, len_i, parts_i, &p_len_i);
    exps_to_partition(exps_j, len_j, parts_j, &p_len_j);

    int sum_parts[NUM_PRIMES];
    int sum_len;
    add_partitions(parts_i, p_len_i,
                   parts_j, p_len_j,
                   sum_parts, &sum_len);

    return partition_to_int(sum_parts, sum_len);
}
