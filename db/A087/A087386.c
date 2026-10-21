/*
 * A087386  a(n) = smallest prime == 1 (mod P(n)) where P(n) is the n-th
 *          Palindrome.
 * https://oeis.org/A087386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest prime r such that r ≡ 1 (mod P(n)), where
 *            P(n) is the n-th decimal palindrome
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   isPrimeULL
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeULL.h"

/* ------------------------------------------------------------
   Construct the n-th palindrome (1‑based).
   Returns a 64‑bit unsigned integer because for n≤10000
   the palindrome fits in 10^10. */
static uint64_t nth_palindrome(idx_t n) {
    static uint64_t pow10[20] = {0};
    static int pow10_inited = 0;

    if (!pow10_inited) {
        pow10[0] = 1;
        for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * 10ULL;
        pow10_inited = 1;
    }

    idx_t len = 1;
    while (1) {
        idx_t k = (len + 1) / 2;                /* digits in the left part */
        uint64_t count = 9ULL * pow10[k-1];     /* count of palindromes of this length */

        if (n > count) {                      /* skip this length group */
            n -= count;
            ++len;
        } else {
            /* offset inside this group (0‑based) */
            uint64_t offset = n - 1;
            uint64_t base  = pow10[k-1];
            uint64_t half  = base + offset;     /* left half including middle if odd */

            /* build the palindrome */
            uint64_t pal = half;
            if (len % 2 == 0) {                /* even length: mirror whole half */
                uint64_t rev = 0, t = half;
                while (t > 0) {
                    rev = rev * 10 + t % 10;
                    t   /= 10;
                }
                pal = pal * pow10[k] + rev;
            } else {                           /* odd length: ignore middle digit */
                uint64_t rev = 0, t = half / 10;
                while (t > 0) {
                    rev = rev * 10 + t % 10;
                    t   /= 10;
                }
                pal = pal * pow10[k-1] + rev;
            }
            return pal;
        }
    }
}

/* ------------------------------------------------------------
   Returns a(n) as a seq_t. The result always fits in uint64_t for
   the tested range, but is cast to seq_t for compatibility with
   arbitrary‑precision builds. */
seq_t a(idx_t n) {
    uint64_t p = nth_palindrome(n);       /* nth palindrome */
    uint64_t r = 0;
    /* Search k ≥ 1 for the smallest prime r = k*p + 1 */
    for (uint64_t k = 1;; ++k) {
        r = k * p + 1;
        if (isPrimeULL(r)) break;
    }
    return (seq_t) r;
}
