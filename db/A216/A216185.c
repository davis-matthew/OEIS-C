/*
 * A216185  a(n) is the smallest odd number k with GCD(k,n)=1 such that all
 *          the powers of 2 mod k are distinct from n mod k, or 0 if n is a
 *          power of 2.
 * https://oeis.org/A216185
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216185: For n ≥ 2, return 0 if n is a power of 2; otherwise return the
 * smallest odd integer k with gcd(k,n)=1 such that for every integer t ≥ 0
 * the value 2^t mod k is different from n mod k. */
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t nseq = (seq_t)n;

    if (is_power_of_two(nseq)) return 0;

    for (seq_t k = 3; ; k += 2) {          /* only odd k */
        if (gcd(k, nseq) != 1) continue;   /* require coprime */

        seq_t nmod = nseq % k;
        if (nmod == 1) continue;          /* 2^0 = 1 would collide */

        seq_t r = 2 % k;                  /* 2^1 mod k */
        bool good = true;

        if (r == nmod) good = false;
        while (good && r != 1) {          /* iterate over the cycle */
            r = (r * (seq_t)2) % k;
            if (r == nmod) good = false;
        }

        if (good) return k;
    }
}
