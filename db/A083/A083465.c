/*
 * A083465  Diagonal of A083464.
 * https://oeis.org/A083465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A083464(n,n) = the n-th smallest palindrome > 1 that is
 *            == 1 (mod n); found by a search over palindromes in increasing
 *            order
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    idx_t need = n;
    idx_t got = 0;
    seq_t lo = (seq_t)1;
    for (int len = 1; ; len++) {
        int odd = len & 1;
        seq_t hi = lo * (seq_t)10;
        for (seq_t h = lo; h < hi; h++) {
            seq_t p = h;
            seq_t t = odd ? h / (seq_t)10 : h;
            while (t > (seq_t)0) {
                p = p * (seq_t)10 + t % (seq_t)10;
                t /= (seq_t)10;
            }
            if (p <= (seq_t)1)
                continue;
            if (p % m == (seq_t)1 % m) {
                got++;
                if (got == need)
                    return p;
            }
        }
        if (odd == 0 || len == 1) {
            /* advance half-length when moving from even len to odd len+1 */
        }
        if (!odd)
            lo = hi;
    }
}
