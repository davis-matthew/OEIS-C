/*
 * A050191  T(n,6), array T as in A050186; a count of aperiodic binary words.
 * https://oeis.org/A050191
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d | gcd(n,6)} mu(d) * C(n/d, 6/d) (n >= 6)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu is the Möbius function: mu(1)=1, mu(2)=-1, mu(3)=-1, mu(6)=+1          */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"

/* gcd with unsigned integers */
static inline idx_t my_gcd(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 6) return 0;                /* value for n < offset is irrelevant */

    /* compute g = gcd(n, 6) */
    idx_t g = my_gcd(n, (idx_t)6);

    seq_t res = 0;

    /* mu values for divisors of 6 */
    /* d = 1  -> mu = +1 */
    if ((g % (idx_t)1) == 0) {
        res += binom(n / 1, 6 / 1);
    }
    /* d = 2  -> mu = -1 */
    if ((g % (idx_t)2) == 0) {
        res -= binom(n / 2, 6 / 2);
    }
    /* d = 3  -> mu = -1 */
    if ((g % (idx_t)3) == 0) {
        res -= binom(n / 3, 6 / 3);
    }
    /* d = 6  -> mu = +1 */
    if ((g % (idx_t)6) == 0) {
        res += binom(n / 6, 6 / 6);
    }

    return res;
}
