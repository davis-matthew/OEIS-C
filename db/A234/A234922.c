/*
 * A234922  Array w(n,h), in which row n shows the weights (defined in
 *          Comments) of the partitions of n, arranged in reverse Mathematica
 *          order.
 * https://oeis.org/A234922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A234922:
    w(n,h) = weight of the h-th partition of n in reverse Mathematica order,
    flattened by rows (offset 1).  The function returns a(idx_t n)
    with n≥1.  This is achieved by mapping the global index to a row
    and a rank in the standard (non‑increasing) partition order. */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_N 600  /* sufficient for all indices that fit in 64‑bit */

static uint64_t part_cnt[MAX_N + 1];             /* p[n] = number of partitions of n   */
static uint64_t part_le[MAX_N + 1][MAX_N + 1];   /* P[n][m] = #partitions of n with parts ≤ m  */

static bool initialized = false;

/* Precompute partition counts and the P table with saturation at UINT64_MAX. */
static void init_tables(void)
{
    int i, j;
    for (j = 0; j <= MAX_N; ++j) part_le[0][j] = 1;  /* empty partition */

    for (i = 1; i <= MAX_N; ++i) {
        part_le[i][0] = 0;
        for (j = 1; j <= MAX_N; ++j) {
            uint64_t val = part_le[i][j-1];
            if (j <= i) {
                uint64_t add = part_le[i-j][j];
                if (UINT64_MAX - val < add)
                    val = UINT64_MAX;
                else
                    val += add;
            }
            part_le[i][j] = val;
        }
        part_cnt[i] = part_le[i][i];
    }
    initialized = true;
}

/* Return the weight of the j-th partition of n in standard (descending)
   lexicographic order (1‑based j).  j is guaranteed ≤ part_cnt[n]. */
static seq_t weight_of_partition(int n, uint64_t j)
{
    int parts[MAX_N];   /* at most n parts; n ≤ MAX_N */
    int plen = 0;
    int remaining = n;
    int max_part = n;
    uint64_t rank = j;

    while (remaining > 0) {
        int start = remaining < max_part ? remaining : max_part;
        for (int first = start; first >= 1; --first) {
            uint64_t count = part_le[remaining - first][first];
            if (rank > count) {
                rank -= count;
            } else {
                parts[plen++] = first;
                remaining -= first;
                max_part = first;
                break;
            }
        }
    }

    seq_t w = 0;
    for (int i = 0; i < plen; ++i) {
        w += (seq_t)(plen - i) * parts[i];
    }
    return w;
}

/* Main function exposed to the OEIS harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */

    if (!initialized) init_tables();

    /* Find the row r such that the cumulative number of partitions
       up to that row is ≥ n.  Since n ≤ 2^64‑1 this is always possible
       with r ≤ MAX_N. */
    unsigned __int128 cumulative = 0;
    int r = 0;
    while (true) {
        ++r;
        if (r > MAX_N) break;           /* safety, should not happen */
        cumulative += (unsigned __int128)part_cnt[r];
        if (cumulative >= (unsigned __int128)n) break;
    }

    /* Rank of the partition within the row, from the end (standard order). */
    unsigned __int128 rank128 = cumulative - (unsigned __int128)n + 1;
    uint64_t rank = (uint64_t)rank128;  /* rank fits in 64 bits */

    return weight_of_partition(r, rank);
}
