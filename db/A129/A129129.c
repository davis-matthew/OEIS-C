/*
 * A129129  Irregular triangle T(n,k) read by rows: Heinz numbers of the
 *          integer partitions of n in graded reverse-lexicographic order, 1
 *          <= k <= A000041(n).
 * https://oeis.org/A129129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Heinz number of the partition of * the integer row r such
 *            that the cumulative number of partitions * up to r exceeds n.
 *            The offset within the row is n - Σ_{i<r} p(i), * where p(i) is
 *            the number of partitions of i. The partition at * that offset is
 *            the offset‑th partition of r in graded rev...
 * Range:     u8 n <= 65, u16 n <= 913, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static idx_t *part_cache = NULL;   /* p(i) for i < part_cache_size */
static size_t   part_cache_size = 0;

/* Extend the partition cache so that part_cache[i] exists for all
   i <= limit.  Uses Euler's pentagonal number theorem. */
static void
ensure_partitions(idx_t limit)
{
    if (part_cache == NULL) {
        part_cache = (idx_t *)malloc(sizeof(idx_t));
        if (!part_cache) return;   /* out of memory – caller will fail */
        part_cache[0] = 1;         /* p(0) = 1, the empty partition */
        part_cache_size = 1;
    }
    while (part_cache_size <= limit) {
        idx_t n   = part_cache_size;
        int64_t sum = 0;
        int sign = 1;
        for (int k = 1; ; ++k) {
            idx_t g1 = (idx_t)(k * (3LL * k - 1) / 2);
            if (g1 > n) break;
            sum += sign * (int64_t)part_cache[n - g1];
            idx_t g2 = (idx_t)(k * (3LL * k + 1) / 2);
            if (g2 <= n) sum += sign * (int64_t)part_cache[n - g2];
            sign = -sign;
        }
        part_cache = (idx_t *)realloc(part_cache, (n + 1) * sizeof(idx_t));
        if (!part_cache) return;   /* out of memory – caller will fail */
        part_cache[n] = (idx_t)sum;
        part_cache_size++;
    }
}

/* Produce the next partition in graded reverse‑lexicographic order.
 * The current partition is stored in parts[0..m-1] (non‑increasing).
 * On success, parts is updated to the next partition and *m is
 * updated; the function returns true.  If there is no next partition,
 * *m is unchanged and the function returns false. */
static bool
next_partition(int *parts, size_t *m)
{
    long i = (long)*m - 1;                       /* rightmost index */
    while (i >= 0 && parts[i] == 1) i--;          /* skip trailing 1's */
    if (i < 0) return false;                     /* all parts are 1 */

    parts[i]--;                                   /* decrement */
    int rem = 0;
    for (size_t j = i + 1; j < *m; ++j) rem += parts[j];
    rem += 1;                                     /* account for the decrement */

    size_t new_m = i + 1;                         /* elements before i+1 are fixed */
    while (rem >= parts[i]) {                     /* repeat the new value */
        parts[new_m++] = parts[i];
        rem -= parts[i];
    }
    if (rem > 0) parts[new_m++] = rem;            /* remaining part */
    *m = new_m;
    return true;
}

/* ---------- the requested sequence function ---------- */
seq_t
a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t row        = 0;

    /* Locate the row r so that Σ_{i<r} p(i) <= n < Σ_{i<=r} p(i) */
    for (;;) {
        ensure_partitions(row);
        if (n < cumulative + part_cache[row]) break;
        cumulative += part_cache[row];
        row++;
    }

    /* Offset within the found row */
    idx_t offset = n - cumulative;

    /* Empty partition (row 0) */
    if (row == 0) return (seq_t)1;

    /* Prepare the first partition of this row: [row] */
    int *parts = (int *)malloc((row + 1) * sizeof(int));
    if (!parts) return (seq_t)0;                  /* memory failure – fallback */
    size_t m = 1;
    parts[0] = (int)row;

    /* Advance offset times to reach the desired partition */
    for (idx_t step = 0; step < offset; ++step) {
        if (!next_partition(parts, &m)) {         /* should never happen */
            free(parts);
            return (seq_t)0;
        }
    }

    /* Compute the Heinz number: product of the primes for each part size */
    seq_t value = (seq_t)1;
    for (size_t i = 0; i < m; ++i) {
        value *= (seq_t)get_prime((idx_t)parts[i]);
    }

    free(parts);
    return value;
}
