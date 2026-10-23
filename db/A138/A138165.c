/*
 * A138165  Prime numbers that contain each of the digits 0,1,4,6,8,9 exactly
 *          once.
 * https://oeis.org/A138165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   compare_seq_t, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime that uses each of the digits 0,1,4,6,8,9 exactly once. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/compare_seq_t.h"

static const int D[6] = {0, 1, 4, 6, 8, 9};

/* Recursively build all 6‑digit permutations of D and store the
   prime ones in *primes.  *cnt tracks the number of primes found. */
static void gen_perm(seq_t cur, int depth, bool used[6],
                     seq_t *primes, size_t *cnt)
{
    if (depth == 6) {
        if (is_prime_small(cur)) {
            primes[*cnt] = cur;
            ++(*cnt);
        }
        return;
    }
    for (int i = 0; i < 6; ++i) {
        if (used[i]) continue;
        if (depth == 0 && D[i] == 0) continue;   /* no leading zero */
        used[i] = true;
        gen_perm(cur * 10 + D[i], depth + 1, used, primes, cnt);
        used[i] = false;
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t primes[200];   /* 200 > 66 total primes */
    static size_t count = 0;

    if (!initialized) {
        bool used[6] = {false};
        gen_perm(0, 0, used, primes, &count);
        qsort(primes, count, sizeof(seq_t), compare_seq_t);
        initialized = true;
    }

    if (n == 0 || (size_t)n > count) return 0;
    return primes[n - 1];
}
