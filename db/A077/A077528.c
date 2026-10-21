/*
 * A077528  a(n) = smallest nontrivial (>1) palindrome == 1 (mod n).
 * https://oeis.org/A077528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = smallest palindrome > 1 such that a(n) ≡ 1 (mod n)
 * Range:     u8 n <= 22, u16 n <= 90, u32 n <= 1088, u64 n <= 9800,
 *            u128 n <= 10000, big any n
 * Helpers:   make_even_pal, make_odd_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/make_even_pal.h"
#include "../utils/make_odd_pal.h"

static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL
};

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined for n < 2 */

    seq_t mod = (seq_t)n;                  /* cast once for modulo */

    for (int len = 1; ; ++len) {
        /* single‑digit palindromes (len == 1) */
        if (len == 1) {
            for (int d = 2; d <= 9; ++d) {
                seq_t p = (seq_t)d;
                if ((p % mod) == (seq_t)1) return p;
            }
            continue;
        }

        /* even length palindromes */
        if ((len % 2) == 0) {
            int halfLen = len / 2;
            uint64_t start = pow10[halfLen - 1];
            uint64_t end   = pow10[halfLen] - 1;
            for (uint64_t half = start; half <= end; ++half) {
                seq_t p = make_even_pal(half);
                if ((p % mod) == (seq_t)1) return p;
            }
        }
        /* odd length palindromes */
        else {
            int halfLen = len / 2;
            uint64_t start = pow10[halfLen - 1];
            uint64_t end   = pow10[halfLen] - 1;
            for (uint64_t half = start; half <= end; ++half) {
                for (int mid = 0; mid <= 9; ++mid) {
                    seq_t p = make_odd_pal(half, mid);
                    if ((p % mod) == (seq_t)1) return p;
                }
            }
        }
        /* safety: if len grows too large, avoid overflow in pow10 */
        if (len > 18) break;        /* unlikely but prevents endless loop */
    }
    return (seq_t)0; /* In practice this line is never reached for n >= 2 */
}
