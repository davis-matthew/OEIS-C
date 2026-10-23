/*
 * A134343  Expansion of psi(-x)^2 in powers of x where psi() is a Ramanujan
 *          theta function.
 * https://oeis.org/A134343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(4*n + 1), where b is multiplicative: b(2^e) = 0^e,
 *            b(p^e) = (1 + (-1)^e)/2 if p ≡ 3 (mod 4), b(p^e) = e + 1 if p ≡
 *            1 (mod 8), b(p^e) = (-1)^e · (e + 1) if p ≡ 5 (mod 8)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   part_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/part_value.h"

/* Factor n into arrays pr[], ex[], with count stored in cnt.
   Arrays should be large enough to hold all prime factors of n.
   For n <= UINT64_MAX minimal size is 64.  */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1)
        return;

    /* factor 2 */
    if ((n & 1) == 0) {
        size_t e = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            ++e;
        }
        pr[*cnt] = 2;
        ex[*cnt] = e;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            size_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            pr[*cnt] = d;
            ex[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t m = 4ULL * n + 1;          /* m = 4*n + 1 */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;

    factor_uint64(m, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = part_value(pr[i], ex[i]);
        if (part == 0)
            return 0;
        res *= part;
    }
    return res;
}
