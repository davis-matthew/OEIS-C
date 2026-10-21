/*
 * A228099  Triangle read by rows, T(n, k) =
 *          prime(1)^p(k,1)*...*prime(n)^p(k,n) where p(k,j) is the j-th part
 *          of the k-th partition of n, additionally T(0,0) = 1. The
 *          partitions of n are ordered such that partitions of n into r parts
 *          appear in lexicographic order previous to the partitions of n into
 *          s parts...
 * https://oeis.org/A228099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 96, u64 n <= 683,
 *            u128 n <= 9295, big any n
 * Helpers:   find_k_partition, gen_fixed_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A228099
    The sequence is the triangle read by rows.  Each row r
    consists of the values
          T(r,k) = prime(0)^p(k,1) * prime(1)^p(k,2) * … * prime(r-1)^p(k,r)
    where p(k,j) is the j‑th part of the k‑th partition of r.
    The partitions are ordered such that partitions with more parts come
    first, and within each group they are in lexicographic ascending order.
    a(n) is the n‑th element in the row‑major scan of the triangle.
    The function below computes a(n) for any n ≥ 0.
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/find_k_partition.h"
#include "../utils/gen_fixed_part.h"

/* ---- small prime table --------------------------------------- */
static const uint32_t primes[30] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};

/* ---- partition count table ------------------------------------ */
static idx_t partCnt[201];
static int  partCnt_ready = 0;

static void init_partition_counts(void) {
    if (partCnt_ready) return;
    const int N = 200;
    for (int i = 0; i <= N; ++i) partCnt[i] = 0;
    partCnt[0] = 1;                      /* p(0) = 1 */
    for (int coin = 1; coin <= N; ++coin) {
        for (int j = coin; j <= N; ++j) {
            partCnt[j] += partCnt[j - coin];
        }
    }
    partCnt_ready = 1;
}

/* ---- cumulative counts of rows (triangular scan) ------------ */
static idx_t cum[201];
static int   cum_len = 0;

static void extend_cum(idx_t target) {
    if (cum_len == 0) {                 /* initialise */
        init_partition_counts();
        cum[0] = partCnt[0];            /* row 0   */
        cum_len = 1;
    }
    while (cum[cum_len - 1] <= target) {
        int r = cum_len;                 /* row to add */
        cum[cum_len] = cum[cum_len - 1] + partCnt[r];
        cum_len++;
    }
}

/* ---- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    extend_cum(n);

    int r = 0;
    while (cum[r] <= n) ++r;
    idx_t prev = (r == 0) ? 0 : cum[r - 1];
    idx_t k = n - prev;

    if (r == 0) return (seq_t)1;            /* a(0) = 1 */

    unsigned char parts[200];
    int len;
    find_k_partition(r, k, parts, &len);

    seq_t prod = 1;
    for (int j = 0; j < r; ++j) {
        unsigned char exp = (j < len) ? parts[j] : 0;
        if (exp) {
            seq_t p = (seq_t)primes[j];
            for (unsigned char e = 0; e < exp; ++e) prod *= p;
        }
    }
    return prod;
}
