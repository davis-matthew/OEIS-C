/*
 * A140695  A number n is included if the number of positive divisors of n is
 *          neither a divisor of n nor is coprime to n.
 * https://oeis.org/A140695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th positive integer whose number of positive divisors
   neither divides the integer nor is coprime to it. */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        uint64_t d = divisor_count_u64(x);
        if (x % d == 0) continue;                   /* d divides x → skip */
        seq_t g = gcd((seq_t)x, (seq_t)d);          /* gcd(d, x) > 1 ? */
        if (g <= 1) continue;                       /* coprime → skip */
        ++count;
        if (count == n) return (seq_t)x;            /* found the n‑th term */
    }
}
