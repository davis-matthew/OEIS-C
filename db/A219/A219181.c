/*
 * A219181  Number of partitions of n into the maximal possible number of
 *          distinct prime parts or 0 if there are no such partitions.
 * https://oeis.org/A219181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 315, u16 n <= 1733, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into the maximal possible number of distinct
 * primes.  If no such partition exists, return 0.
 * This implementation performs a subset‑sum DP over all primes ≤ n,
 * keeping for each sum the largest number of distinct primes that
 * can attain that sum and how many ways achieve it.
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* 0 has one folding: empty partition */
    if (n == 0)
        return (seq_t)1;

    /* Generate all primes ≤ n once per call. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);
    if (prime_cnt == 0) {
        /* n < 2: cannot partition into primes */
        return (seq_t)0;
    }

    /* dp arrays: best_k[s] = maximum number of primes that sum to s,
     * cnt[s]      = number of subsets that reach s with that maximum.
     */
    size_t N = (size_t)n;
    int  *best_k = (int *)malloc((N + 1) * sizeof(int));
    seq_t *cnt   = (seq_t *)calloc((N + 1), sizeof(seq_t));
    if (!best_k || !cnt) {
        free(best_k);
        free(cnt);
        free(primes);
        return (seq_t)0;   /* allocation failure: conservatively return 0 */
    }

    for (size_t i = 0; i <= N; ++i)
        best_k[i] = -1;
    best_k[0] = 0;          /* sum 0 uses 0 primes */
    cnt[0]    = (seq_t)1;

    /* Process each prime, updating DP in reverse order to avoid reuse. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        size_t p = (size_t)primes[pi];
        if (p > N) break;
        for (size_t s = N; s >= p; --s) {
            int prev = best_k[s - p];
            if (prev < 0) continue;            /* unreachable sum */
            int new_k = prev + 1;
            if (new_k > best_k[s]) {
                best_k[s] = new_k;
                cnt[s]   = cnt[s - p];
            } else if (new_k == best_k[s]) {
                cnt[s] += cnt[s - p];
            }
        }
    }

    seq_t result = (best_k[N] > 0) ? cnt[N] : (seq_t)0;
    free(best_k);
    free(cnt);
    free(primes);
    return result;
}
