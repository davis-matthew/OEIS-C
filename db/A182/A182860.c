/*
 * A182860  Number of distinct prime signatures represented among the unitary
 *          divisors of n.
 * https://oeis.org/A182860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime signatures represented among the
 *            unitary divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* sort a small array of int in ascending order */
static void sort_ints(int *a, int n)
{
    for (int i = 1; i < n; ++i) {
        int v = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > v) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = v;
    }
}

/* Return a(n) for OEIS A182860 */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* containers for unique signatures */
    int uniq_len[256];
    int uniq_sig[256][32];
    size_t uniq_count = 0;

    /* enumerate all unitary divisors (subsets of prime factors) */
    uint64_t limit = (cnt == 0) ? 1ULL : (1ULL << cnt);
    for (uint64_t mask = 0; mask < limit; ++mask) {
        int exlist[32];
        int exlen = 0;

        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                exlist[exlen++] = (int)exps[i];
            }
        }

        if (exlen > 1) {
            sort_ints(exlist, exlen);
        }

        /* check whether this signature is already counted */
        bool found = false;
        for (size_t j = 0; j < uniq_count; ++j) {
            if (uniq_len[j] == exlen &&
                memcmp(uniq_sig[j], exlist, sizeof(int) * exlen) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            uniq_len[uniq_count] = exlen;
            memcpy(uniq_sig[uniq_count], exlist, sizeof(int) * exlen);
            ++uniq_count;
        }
    }

    return (seq_t)uniq_count;
}
