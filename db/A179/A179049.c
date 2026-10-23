/*
 * A179049  Odd-even partitions: number of partitions into distinct parts
 *          where all differences between consecutive parts are odd and the
 *          minimal part is odd.
 * https://oeis.org/A179049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 134, u32 n <= 402, u64 n <= 1331,
 *            u128 n <= 4724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A179049: odd–even partitions
 * Number of partitions of n into distinct parts where consecutive
 * differences are odd and the minimal part is odd.
 *
 * The generating function gives the following concise formula:
 *   a(n) = Σ_{m ≥ 0} P( (n - m(m+1)/2)/2 , m )
 * where P(s, k) is the number of partitions of s whose largest part
 * does not exceed k (equivalently, partitions of s into at most k parts).
 *
 * The implementation builds a 2‑D table of P(s, k) up to the
 * required bounds on demand.  For n ≤ 10 000 the table size is
 * comfortably within the limits of the static arrays below.
 *
 * Complexity: the table is rebuilt in O(s_max * m_max) ≈ O(n^(3/2))
 * time the first time a value beyond the current maximum is needed.
 * Each query thereafter runs in O(√n) time.
 */

#include <stdint.h>
#include <stddef.h>

/* Maximum n for which the program is designed to run efficiently.
 * The constants below are chosen so that all needed entries of the
 * partition table fit in the static arrays for n ≤ 10 000. */
#define MAX_S 5000   /* floor(10000/2) */
#define MAX_M 150    /* ceil(sqrt(2*10000)) + safety margin */

/* Static table for partition numbers:  P[s][k] = partitions of s
 * into parts of size at most k (i.e. at most k parts). */
static seq_t partition_table[MAX_S + 1][MAX_M + 1];

/* Largest n, m and s for which the table has been constructed. */
static idx_t table_max_n = 0;
static idx_t table_max_m = 0;
static idx_t table_max_s = 0;

/* Rebuild the partition table so that it covers all sums up to
 * n_max / 2 and all maximum part sizes up to the index of the
 * largest triangular number ≤ n_max. */
static void rebuild_partition_table(idx_t n_max)
{
    /* Determine the needed ranges. */
    idx_t m_max = 0;
    while ((m_max + 1) * (m_max + 2) / 2 <= n_max) {
        ++m_max;
    }
    if (m_max > MAX_M - 1) m_max = MAX_M - 1;      /* safety */
    idx_t s_max = n_max / 2;
    if (s_max > MAX_S) s_max = MAX_S;              /* safety */

    /* Base case: empty partition. */
    for (idx_t k = 0; k <= m_max; ++k) {
        partition_table[0][k] = 1;
    }
    /* Zero out impossible positive sums with zero parts. */
    for (idx_t s = 1; s <= s_max; ++s) {
        partition_table[s][0] = 0;
    }

    /* Build the rest of the table using the recurrence
     *   P[s][k] = P[s][k-1] + P[s-k][k]  (if s ≥ k).
     */
    for (idx_t k = 1; k <= m_max; ++k) {
        for (idx_t s = 1; s <= s_max; ++s) {
            seq_t val = partition_table[s][k - 1];
            if (s >= k) {
                val += partition_table[s - k][k];
            }
            partition_table[s][k] = val;
        }
    }

    /* Record the new bounds. */
    table_max_n = n_max;
    table_max_m = m_max;
    table_max_s = s_max;
}

/* Main sequence function, as required by the build. */
seq_t a(idx_t n)
{
    /* Build or extend the table on the first call or when n exceeds
     * the current maximum. */
    if (n > table_max_n || table_max_n == 0) {
        rebuild_partition_table(n);
    }

    seq_t sum = 0;
    idx_t m_max = table_max_m;

    for (idx_t m = 0; m <= m_max; ++m) {
        idx_t tri = m * (m + 1) / 2;
        if (tri > n) break;          /* further m only increase tri */

        idx_t diff = n - tri;
        if (diff & 1) continue;      /* require even remainder */

        idx_t s = diff / 2;
        /* The table entry contains the number of partitions of s
         * into parts of size at most m (equivalently, at most m parts). */
        sum += partition_table[s][m];
    }

    return sum;
}
