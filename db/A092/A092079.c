/*
 * A092079  Characteristic array marking partitions of m whose parts are
 *          exponents of partitions of n into m parts.
 * https://oeis.org/A092079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A092079: Characteristic array marking partitions of m whose parts are exponents
   of partitions of n into m parts.
   Implementation uses a pre‑computation of all partitions up to 25
   and a lookup table of realizable multiplicity patterns. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 25                    /* maximum n (and m) needed for indices ≤ 10000 */

typedef struct {
    int len;                         /* number of parts in the partition   */
    unsigned char vals[MAX_N];       /* partition parts (descending order) */
} Partition;

/* global tables ---------------------------------------------------------- */
static Partition *partitions_of_m[MAX_N + 1];   /* partitions of each m      */
static int part_count[MAX_N + 1];              /* number of partitions of m */
static bool **realiz[MAX_N + 1];               /* realizable flags [n][m][k] */

/* auxiliary recursion to generate all partitions of n (any length)          */
static void gen_parts_all(int rem, int maxv, unsigned char *buf, int pos,
                          Partition *list, int *cnt)
{
    if (rem == 0) {
        list[*cnt].len = pos;
        memcpy(list[*cnt].vals, buf, pos);
        (*cnt)++;
        return;
    }
    int vmax = rem < maxv ? rem : maxv;
    for (int v = vmax; v >= 1; --v) {
        buf[pos] = (unsigned char)v;
        gen_parts_all(rem - v, v, buf, pos + 1, list, cnt);
    }
}

/* auxiliary recursion to enumerate partitions of n into exactly m parts     */
static void enum_parts_exact(int rem, int left, int maxv,
                             unsigned char *buf, int pos,
                             int n_outer, int m_outer, bool *flag_arr)
{
    if (left == 0) {
        if (rem != 0) return; /* finished parts but sum not n, discard */
        /* compute multiplicity vector of the partition in buf (size m_outer) */
        int freq[MAX_N + 1] = {0};
        for (int i = 0; i < m_outer; ++i) freq[buf[i]]++;
        int mult[MAX_N + 1], mm = 0;
        for (int v = 1; v <= n_outer; ++v)
            if (freq[v]) mult[mm++] = freq[v];

        /* sort multiplicities descending */
        for (int i = 0; i < mm - 1; ++i)
            for (int j = i + 1; j < mm; ++j)
                if (mult[i] < mult[j]) {
                    int t = mult[i];
                    mult[i] = mult[j];
                    mult[j] = t;
                }

        /* find index of this multiplicity vector in partition list of m_outer */
        for (int k = 0; k < part_count[m_outer]; ++k) {
            if (partitions_of_m[m_outer][k].len != mm) continue;
            bool eq = true;
            for (int t = 0; t < mm; ++t)
                if (partitions_of_m[m_outer][k].vals[t] != mult[t]) {
                    eq = false;
                    break;
                }
            if (eq) {
                flag_arr[k] = true;
                break;
            }
        }
        return;
    }
    int vmax = rem < maxv ? rem : maxv;
    /* ensure that enough sum can be left for remaining parts */
    for (int v = vmax; v >= 1; --v) {
        if (rem - v < left - 1) continue;
        buf[pos] = (unsigned char)v;
        enum_parts_exact(rem - v, left - 1, v, buf, pos + 1,
                         n_outer, m_outer, flag_arr);
    }
}

/* pre‑compute all needed data ------------------------------------------- */
static void precompute(void)
{
    unsigned char buf[MAX_N];
    /* Generate partitions for each m (1..MAX_N) */
    for (int m = 1; m <= MAX_N; ++m) {
        part_count[m] = 0;
        part_list_alloc:
        partitions_of_m[m] = (Partition *)malloc(2000 * sizeof(Partition));
        if (!partitions_of_m[m]) abort();          /* allocation failure */
        gen_parts_all(m, m, buf, 0, partitions_of_m[m], &part_count[m]);
    }

    /* Allocate realizable flag arrays (n from 1..MAX_N, m from 1..n) */
    for (int n = 1; n <= MAX_N; ++n) {
        realiz[n] = (bool **)malloc((n + 1) * sizeof(bool *));
        if (!realiz[n]) abort();
        for (int m = 1; m <= n; ++m) {
            realiz[n][m] = (bool *)calloc(part_count[m], sizeof(bool));
            if (!realiz[n][m]) abort();
            /* enumerate partitions of n into m parts and mark realizable ones */
            enum_parts_exact(n, m, n, buf, 0, n, m, realiz[n][m]);
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        precompute();
        initialized = true;
    }
    if (n == 0) return (seq_t)0; /* safety, though not expected */

    idx_t idx = n - 1;                /* 0‑based index in flattened array   */
    for (int n_part = 1; n_part <= MAX_N; ++n_part) {
        for (int m_part = 1; m_part <= n_part; ++m_part) {
            int len = part_count[m_part];
            if (idx < (idx_t)len) {          /* found the block */
                return (seq_t)realiz[n_part][m_part][(int)idx];
            }
            idx -= (idx_t)len;
        }
    }
    /* If we reach here the requested index exceeds precomputed range.
       For safety return 0, though such a case should not arise with given limits. */
    return (seq_t)0;
}
