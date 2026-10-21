/*
 * A064380  Number of numbers less than n that are infinitarily relatively
 *          prime to n; the infinitary Euler phi function.
 * https://oeis.org/A064380
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = The number of 1 ≤ k < n such that for every prime * p
 *            dividing n, the exponent of p in k and in n contain no common *
 *            binary 1‑bits; equivalently, (v_p(k) & v_p(n)) == 0 for all p|n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* factor n into prime powers p^e */
    idx_t primes[16];
    idx_t exps[16];
    size_t pc = 0;
    idx_t tmp = n;
    for (idx_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            idx_t e = 0;
            while (tmp % d == 0) {
                ++e;
                tmp /= d;
            }
            primes[pc] = d;
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc] = tmp;
        exps[pc] = 1;
        ++pc;
    }

    /* count integers k with the required property */
    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        bool ok = true;
        for (size_t i = 0; i < pc && ok; ++i) {
            idx_t p  = primes[i];
            idx_t eN = exps[i];
            idx_t f  = 0;
            idx_t tk = k;
            while (tk % p == 0) {
                ++f;
                tk /= p;
            }
            if ((f & eN) != 0) ok = false;
        }
        if (ok) ++count;
    }
    return count;
}
