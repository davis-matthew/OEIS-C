/*
 * A227781  Least number of squares which add to -1 mod n.
 * https://oeis.org/A227781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1: if n%8==0 -> 4; else if n%4==0 -> 3; else if
 *            all odd prime factors of n ≡1 (mod4) -> 1; else -> 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9731 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool odd_factors_all_one_mod_four(idx_t n) {
    /* n is guaranteed to be odd (or 1). */
    if (n <= 1) return true;
    idx_t d = 3;
    /* trial division up to sqrt(n) using 128‑bit comparison to avoid overflow */
    while ((__uint128_t)d * d <= n) {
        if (n % d == 0) {
            if (d % 4 != 1) return false;
            while (n % d == 0) n /= d;
        }
        d += 2;
    }
    /* remaining prime factor, if any */
    if (n > 1) return (n % 4 == 1);
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;

    /* Handle even values first */
    if ((n & 1) == 0) {
        if ((n & 3) == 0) { /* divisible by 4 */
            if ((n & 7) == 0) return (seq_t)4;   /* n % 8 == 0 */
            return (seq_t)3;                     /* n % 4 == 0 but n % 8 != 0 */
        }
        /* n is 2 mod 4 */
        /* Remove all factors of 2 to get the odd part */
        idx_t odd = n;
        while ((odd & 1) == 0) odd >>= 1;
        if (odd_factors_all_one_mod_four(odd)) return (seq_t)1;
        return (seq_t)2;
    }

    /* n is odd */
    if (odd_factors_all_one_mod_four(n)) return (seq_t)1;
    return (seq_t)2;
}
