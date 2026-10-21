/*
 * A072811  T(n,k) = multiplicity of the k-th partition of n in Mathematica
 *          order, defined to be the count of its permutations (compositions).
 * https://oeis.org/A072811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look, tabf
 * Formula:   a(n) = multinomial coefficient of the signature of the n‑th
 *            partition in the global list of all integer partitions (ordered
 *            by the integer being partitioned and, for each integer, by
 *            Mathematica lexicographic order)
 * Range:     u8 n <= 42, u16 n <= 1964, u32 n <= 3498, u64 n <= 9554,
 *            u128 n <= 9554, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdbool.h>     /* bool */
#include <string.h>      /* memcpy, memset */
#include "../utils/factorial.h"

#define MAXM 200          /* safe upper bound for m when n <= 10000 */

/* static buffer used by the recursive k‑th partition finder */
static int current_parts[MAXM];

/* recursive helper to enumerate partitions of 'rem' with parts <= 'max'
   in Mathematica order (decreasing parts).  'k' is the remaining index
   (passed by pointer); when a partition is found it is copied into 'out'.
   Returns true when the desired partition has been found.                */
static bool rec_kth(int rem, int max, unsigned int *k,
                    int out[200], size_t *len, size_t depth)
{
    if (rem == 0) {
        if (*k == 0) {
            *len = depth;
            memcpy(out, current_parts, depth * sizeof(int));
            return true;
        } else {
            (*k)--;
            return false;
        }
    }
    for (int part = max; part >= 1; --part) {
        if (part > rem) continue;
        current_parts[depth] = part;
        if (rec_kth(rem - part, part, k, out, len, depth + 1))
            return true;
    }
    return false;
}

/* Finds the k‑th (0‑based) partition of the integer 'm' and returns
   the partition in 'out' (length stored in 'len').                      */
static bool find_kth_partition(int m, unsigned int k,
                               int out[200], size_t *len)
{
    return rec_kth(m, m, &k, out, len, 0);
}

/* Main function: returns the a(n) value as defined above.                 */
seq_t a(idx_t n)
{
    /* ---- 1. Determine which integer 'm' is being partitioned ---- */
    static idx_t dp[MAXM + 1];
    idx_t cumulative = 0;
    size_t m = 0;

    while (1) {
        /* Recompute partition numbers up to current 'm' */
        memset(dp, 0, (m + 1) * sizeof(idx_t));
        dp[0] = 1;
        for (size_t part = 1; part <= m; ++part) {
            for (size_t sum = part; sum <= m; ++sum) {
                dp[sum] += dp[sum - part];
            }
        }
        cumulative += dp[m];

        if (cumulative > n) break;
        ++m;
        if (m > MAXM) break;  /* safety check */
    }

    /* ---- 2. Compute local index 'k' within partitions of 'm' ---- */
    idx_t prev_cumulative = cumulative - dp[m];
    idx_t k = n - prev_cumulative;

    /* Empty partition case (m == 0) */
    if (m == 0) return 1;

    /* ---- 3. Find the k-th partition of 'm' ---- */
    int parts[200];
    size_t parts_len = 0;
    unsigned int k_unsigned = (unsigned int)k;

    if (!find_kth_partition((int)m, k_unsigned, parts, &parts_len))
        return 0;   /* should not happen */

    /* ---- 4. Compute the multinomial coefficient (number of permutations) ---- */
    size_t total_parts = parts_len;
    if (total_parts == 0) return 1;

    /* Count equal adjacent parts to get the multiplicity vector */
    unsigned int counts[200];
    size_t count_len = 0;
    int cur_part = parts[0];
    unsigned int dup = 1;
    for (size_t i = 1; i < parts_len; ++i) {
        if (parts[i] == cur_part) {
            ++dup;
        } else {
            counts[count_len++] = dup;
            cur_part = parts[i];
            dup = 1;
        }
    }
    counts[count_len++] = dup;   /* last group */

    /* Multinomial: (total_parts)! / product(counts[i]!) */
    seq_t result = factorial(total_parts);
    for (size_t i = 0; i < count_len; ++i) {
        result /= factorial(counts[i]);
    }

    return result;
}
