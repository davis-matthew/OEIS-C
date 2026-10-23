/*
 * A173482  a(n) = the smallest (n+1)-digit number ending in n zeros that is
 *          divisible by n, else 0.
 * https://oeis.org/A173482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest (n+1)-digit number ending in n zeros that is
 *            * divisible by n, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused; offset starts at 1 */

    /* compute t = 10^n (mod n) */
    idx_t t = 1;
    idx_t base = 10 % n;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            t = (t * base) % n;
        }
        exp >>= 1;
        if (exp) base = (base * base) % n;
    }

    /* minimal k * 10^n divisible by n  →  k = l = n / gcd(t,n) */
    idx_t d = gcd_idx(t, n);
    idx_t l = n / d;

    if (l > 9) return 0;    /* no one‑digit multiplier fits */

    seq_t pow10n = seq_pow(10, n);
    seq_t k = (seq_t)l;
    return seq_mul(pow10n, k);
}
