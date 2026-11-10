/*
 * A204986  Least k such that n divides A204985(k), the k-th difference
 *          between numbers of the form 2^k.
 * https://oeis.org/A204986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (i-1)*(i-2)/2 + j with j = max(1, v2(n)), i = j +
 *            ord_m(2), where n = 2^v2(n)*m and m is odd (ord_1(2) = 1)
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }

    /* multiplicative order of 2 modulo odd m */
    idx_t L = 1;
    if (m > 1) {
        idx_t r = 2 % m;
        L = 1;
        while (r != 1) {
            r = (idx_t)(((unsigned __int128)r * 2) % m);
            ++L;
        }
    }

    idx_t j = (e < 1) ? 1 : e;
    idx_t i = j + L;

    seq_t res = (seq_t)(i - 1) * ((seq_t)i - 2) / 2 + (seq_t)j;
    return res;
}
