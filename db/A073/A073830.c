/*
 * A073830  a(n) = 4*((n-1)! + 1) + n (mod n*(n + 2)).
 * https://oeis.org/A073830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4 * ((n-1)! + 1) + n) mod (n * (n + 2))
 * Range:     u8 n <= 12, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9965 (2026-11-01)
 * Helpers:   mulmod_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulmod_idx_to_seq.h"

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) is 0, and n >= 1 is the only offset needed */
        return 0;

    seq_t mod = (seq_t)n * ((seq_t)n + 2);   /* n * (n + 2) */

    /* compute (n-1)! modulo mod */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i)
        fact = mulmod_idx_to_seq(fact, i, mod);

    /* a(n) = 4*((n-1)! + 1) + n (mod mod) */
    seq_t value = 4 * ((seq_t)fact + 1) + n;
    return value % mod;
}
