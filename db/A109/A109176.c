/*
 * A109176  Five-digit primes which use each of the decimal digits 0 through 4
 *          exactly once.
 * https://oeis.org/A109176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating all five‑digit primes that use each of the digits 0‑4 exactly once.
   The function enumerates all permutations, filters primes, sorts them,
   and returns the n‑th term (n ≥ 1).  No table is hard‑coded; the result
   is computed on the first call and memoised for subsequent calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t) */

static void permute_rec(seq_t cur, int depth, bool *used, seq_t *out, size_t *cnt)
{
    if (depth == 5) {
        /* Safe cast: all results are < 50000, fit in uint64_t */
        if (is_prime((uint64_t)cur))
            out[(*cnt)++] = cur;
        return;
    }
    for (int d = 0; d < 5; ++d) {
        if (!used[d]) {
            if (depth == 0 && d == 0)            /* no leading zero */
                continue;
            used[d] = true;
            permute_rec(cur * 10 + d, depth + 1, used, out, cnt);
            used[d] = false;
        }
    }
}

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t primes[120];
    static size_t prime_cnt = 0;

    if (!init) {
        bool used[5] = {false};
        permute_rec(0, 0, used, primes, &prime_cnt);
        qsort(primes, prime_cnt, sizeof(seq_t), cmp_seq);
        init = true;
    }

    if (n == 0 || !init || (size_t)n > prime_cnt)
        return (seq_t)0;        /* undefined for n outside the range */

    return primes[(size_t)n - 1];
}
