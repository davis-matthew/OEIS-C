/*
 * A190911  Least number coprime to n and n+3.
 * https://oeis.org/A190911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least integer m>1 such that gcd(n,m)=1 and gcd(n+3,m)=1
 * Range:     u8 n <= 285, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;           /* n as seq_t */
    seq_t n_plus3 = n_val + 3;        /* (n+3) as seq_t */
    seq_t candidate = 2;              /* start from 2 */

    for (;;) {
        if (gcd(n_val, candidate) == 1 && gcd(n_plus3, candidate) == 1)
            return candidate;         /* found the least coprime */
        candidate++;                  /* try next integer */
    }
}
