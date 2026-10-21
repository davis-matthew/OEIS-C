/*
 * A056623  If n=LLgggf (see A056192) and a(n) = LL, then its complementary
 *          divisor n/LL = gggf and gcd(L^2, n/LL) = 1.
 * https://oeis.org/A056623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e || n} p^f where * f = 0 if e = 1 or e = 3 *
 *            f = e if e is even * f = e - 3 if e is odd and > 1 * (i.e.
 *            a(p^e) = p^e for even e, a(p) = 1, a(p^e) = p^(e-3) for odd e >
 *            1)
 * Range:     u8 n <= 255, u16 n <= 9579, u32 n <= 9579, u64 n <= 9579,
 *            u128 n <= 9579, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* safe guard, offset starts at 1 */
    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    int f = 0;
    if (e == 1)          f = 0;
    else if (e % 2 == 0) f = e;
    else                 f = e - 3;        /* e odd > 1; for e==3 f==0 */
    if (f > 0) {
        seq_t sp = (seq_t)2;
        for (int i = 0; i < f; ++i) result *= sp;
    }

    /* handle odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (e == 0) continue;   /* not a factor */
        if (e == 1) continue;   /* f = 0, no contribution */
        if (e % 2 == 0) f = e;
        else           f = e - 3;   /* odd > 1 */
        if (f > 0) {
            seq_t sp = (seq_t)p;
            for (int i = 0; i < f; ++i) result *= sp;
        }
    }

    /* any remaining prime > sqrt(original n) has exponent 1 → contributes nothing */
    return result;
}
