/*
 * A100859  Beginning with 3, increasing primes such that no two adjacent
 *          terms are congruent mod 4.
 * https://oeis.org/A100859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes starting at 3, alternating residues mod 4.
 * a(1)=3. For n>1, a(n) is the smallest prime >a(n‑1) whose residue
 * modulo 4 differs from that of a(n‑1).  Thus odd indices are ≡3 (mod 4),
 * even indices are ≡1 (mod 4). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;               /* first term */

    idx_t   count = 1;                          /* we already have the first term */
    uint64_t cand  = 5;                         /* next odd candidate */
    bool    expect_three = false;               /* expect residue 1 (mod 4) next */

    for (;;)
    {
        /* Skip candidates that do not have the required residue. */
        if ((cand & 3ULL) == (expect_three ? 3ULL : 1ULL))
        {
            if (is_prime(cand))
            {
                ++count;
                if (count == n)
                    return (seq_t)cand;
                expect_three = !expect_three;   /* alternate the required residue */
            }
        }
        cand += 2;                              /* only odd numbers can be prime >2 */
    }
}
