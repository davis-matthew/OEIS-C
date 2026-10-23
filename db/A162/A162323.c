/*
 * A162323  a(n) = the smallest positive integer such that when a(n) is
 *          represented in binary, it contains the binary representations of
 *          every prime dividing n as substrings.
 * https://oeis.org/A162323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest positive integer whose binary representation
 *            contains the binary representations of all distinct prime
 *            divisors of n as substrings
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_u64, is_substring, prime_to_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/is_substring.h"
#include "../utils/prime_to_bin.h"

/* Depth‑first search for the shortest superstring that contains all
   patterns.  The arguments are the same as in the original
   implementation, but now the `primes_cnt` corresponds to the
   number of *kept* patterns after elimination of subsumed ones. */
static void dfs_rec(int used,
                    char *cur, int cur_len,
                    int primes_cnt,
                    char (*patterns)[64], int *patlen,
                    char *best, int *best_len, bool *found)
{
    if (used == (1 << primes_cnt) - 1) {
        if (!*found || cur_len < *best_len ||
            (cur_len == *best_len && memcmp(cur, best, cur_len) < 0)) {
            memcpy(best, cur, cur_len);
            *best_len = cur_len;
            *found = true;
        }
        return;
    }

    /* Prune when the current length already exceeds the best found length. */
    if (*found && cur_len >= *best_len)
        return;

    for (int i = 0; i < primes_cnt; ++i) {
        if (used & (1 << i))
            continue;

        /* Compute the longest possible overlap between the suffix of
           current string and the prefix of patterns[i]. */
        int k = 0;
        int maxk = cur_len < patlen[i] ? cur_len : patlen[i];
        for (k = maxk; k > 0; --k) {
            if (memcmp(cur + cur_len - k, patterns[i], k) == 0)
                break;
        }

        int new_len = cur_len + patlen[i] - k;
        char next[256];
        if (cur_len > 0)
            memcpy(next, cur, cur_len);
        memcpy(next + cur_len, patterns[i] + k, patlen[i] - k);

        dfs_rec(used | (1 << i), next, new_len,
                primes_cnt, patterns, patlen,
                best, best_len, found);
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factor n into distinct prime factors. */
    uint64_t primes[16];
    int primes_cnt = 0;
    uint64_t pr[16], ex[16];
    size_t factor_cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &factor_cnt);
    for (size_t i = 0; i < factor_cnt; ++i) {
        primes[primes_cnt++] = pr[i];
    }

    /* Convert each prime to its binary pattern. */
    char patterns[16][64];
    int patlen[16];
    for (int i = 0; i < primes_cnt; ++i) {
        prime_to_bin(primes[i], patterns[i], &patlen[i]);
    }

    /* Remove patterns that are substrings of another pattern. */
    bool keep[16];
    for (int i = 0; i < primes_cnt; ++i) keep[i] = true;
    for (int i = 0; i < primes_cnt; ++i) {
        if (!keep[i]) continue;
        for (int j = 0; j < primes_cnt; ++j) {
            if (i == j) continue;
            if (!keep[j]) continue;
            if (is_substring(patterns[i], patlen[i], patterns[j], patlen[j])) {
                keep[i] = false;
                break;
            }
        }
    }

    /* Build arrays of patterns that remain. */
    char kept_patterns[16][64];
    int kept_patlen[16];
    int kept_cnt = 0;
    for (int i = 0; i < primes_cnt; ++i) {
        if (keep[i]) {
            memcpy(kept_patterns[kept_cnt], patterns[i], patlen[i] + 1);
            kept_patlen[kept_cnt] = patlen[i];
            ++kept_cnt;
        }
    }

    /* Special case: if after removal no patterns remain (only possible for n=1). */
    if (kept_cnt == 0)
        return (seq_t)1;

    /* Search for the shortest superstring. */
    char best[256];
    int best_len = 1000;   /* intentionally large */
    bool found = false;

    dfs_rec(0, best, 0, kept_cnt,
            kept_patterns, kept_patlen,
            best, &best_len, &found);

    /* Convert the best binary string to a seq_t integer. */
    seq_t result = 0;
    for (int i = 0; i < best_len; ++i) {
        result = (result << 1) | (seq_t)(best[i] - '0');
    }
    return result;
}
