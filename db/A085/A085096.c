/*
 * A085096  Index of the first occurrence of n in A082744, or 0 if n does not
 *          occur in the sequence A082744.
 * https://oeis.org/A085096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that A082744(k) = n, i.e. n*k+1 is a
 *            palindrome and j*k+1 is not a palindrome for 1 <= j < n; 0 if
 *            none is found. Search: enumerate decimal palindromes P in
 *            increasing order, take P == 1 (mod n), k = (P-1)/n, and test the
 *            smaller multipliers
 * Range:     u8 n <= 10, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static bool A085096_is_pal_u64(uint64_t x)
{
    uint64_t r = 0, t = x;
    while (t > 0) { r = r * 10 + t % 10; t /= 10; }
    return r == x;
}

seq_t a(idx_t n)
{
    const int MAXLEN = 14;
    uint64_t nn = (uint64_t)n;
    for (int L = 1; L <= MAXLEN; ++L) {
        int half = (L + 1) / 2;
        uint64_t lo = 1, shift = 1;
        for (int i = 1; i < half; ++i) lo *= 10;
        for (int i = 0; i < L - half; ++i) shift *= 10;
        uint64_t hi = lo * 10;
        for (uint64_t h = lo; h < hi; ++h) {
            uint64_t t = (L & 1) ? h / 10 : h;
            uint64_t rev = 0;
            while (t > 0) { rev = rev * 10 + t % 10; t /= 10; }
            uint64_t P = h * shift + rev;
            if (P < 2) continue;
            if ((P - 1) % nn != 0) continue;
            uint64_t k = (P - 1) / nn;
            if (k == 0) continue;
            bool ok = true;
            for (uint64_t j = 1; j < nn; ++j) {
                if (A085096_is_pal_u64(j * k + 1)) { ok = false; break; }
            }
            if (ok) return (seq_t)k;
        }
    }
    return (seq_t)0;
}
