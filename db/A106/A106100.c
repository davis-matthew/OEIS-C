/*
 * A106100  Primes with maximal digit = 2.
 * https://oeis.org/A106100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 2282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106100: primes whose decimal digits have maximal digit 2
   (i.e., the decimal representation uses only the digits 0,1,2 and
   contains at least one 2).  The function returns the n‑th such prime
   (a(1) = 2). */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

static bool dfs_generate(seq_t current, int pos, int len,
                         bool has2, idx_t *count,
                         seq_t *out, idx_t target)
{
    /* When the full length is reached, check primality and digit rule. */
    if (pos == len) {
        if (has2 && is_prime((uint64_t)current)) {
            (*count)++;
            if (*count == target) {
                *out = current;
                return true;
            }
        }
        return false;
    }

    /* Try digits 0,1,2, but avoid leading zero. */
    for (int d = 0; d <= 2; ++d) {
        if (pos == 0 && d == 0)      /* no leading zero */
            continue;
        seq_t next = current * 10 + (seq_t)d;
        if (dfs_generate(next, pos + 1, len,
                         has2 || (d == 2),
                         count, out, target))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n ≥ 1. Behaviour for n = 0 is not critical. */
    if (n == 0)
        return 0ULL;

    idx_t count = 0;
    seq_t result = 0;

    /* Generate numbers by increasing decimal length. */
    for (int len = 1; ; ++len) {
        if (dfs_generate(0, 0, len, false, &count, &result, n))
            break;
    }

    return result;
}
