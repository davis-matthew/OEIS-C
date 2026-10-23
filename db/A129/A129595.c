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
 * Helpers:   add_partitions, anticlique_coords, big_pow, exps_to_partition,
 *            seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129595: a(n) = integer obtained by element‑wise adding the partitions
 * encoded by the two indices of the n‑th anti‑diagonal of the array, then
 * converting the resulting partition back to an integer.  Indices start at 1.
 */
#include <stdint.h>
#include <math.h>

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"
#include "../utils/add_partitions.h"
#include "../utils/anticlique_coords.h"
#include "../utils/exps_to_partition.h"

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
        seq_t p = big_pow((seq_t)PRIMES[i], (seq_t)exps[i]);
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
