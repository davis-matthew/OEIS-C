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
 * Validated: standard, pass, 65 terms through n = 8984 (2026-11-01)
 * Helpers:   none
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

/* ---- generate a partition of r into m parts (lexicographically) */
static void gen_fixed_part(int parts_left, int rem, int max_part,
                           unsigned char *parts, int idx,
                           idx_t *kptr, unsigned char *out, int *out_len,
                           bool *found) {
    if (*found) return;
    if (parts_left == 0) {
        if (rem == 0) {
            if (*kptr == 0) {
                memcpy(out, parts, idx);
                *out_len = idx;
                *found = true;
            } else {
                (*kptr)--;
            }
        }
        return;
    }
    int max_possible = rem - (parts_left - 1);          /* each remaining part ≥ 1 */
    int upper = max_part;
    if (max_possible < upper) upper = max_possible;
    for (int part = 1; part <= upper; ++part) {
        parts[idx] = (unsigned char)part;
        gen_fixed_part(parts_left - 1, rem - part, part,
                       parts, idx + 1, kptr, out, out_len, found);
        if (*found) return;
    }
}

static bool find_k_partition(int r, idx_t k, unsigned char *out, int *out_len) {
    if (r == 0) { *out_len = 0; return true; }
    unsigned char parts[200];
    bool found = false;
    for (int m = r; m >= 1 && !found; --m) {
        gen_fixed_part(m, r, r, parts, 0, &k, out, out_len, &found);
    }
    return found;
}

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
