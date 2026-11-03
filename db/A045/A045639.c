/*
 * A045639  Palindromic and divisible by 4.
 * https://oeis.org/A045639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑negative integer that is palindromic and
 *            divisible by 4 (base 10)
 * Range:     u8 n <= 8, u16 n <= 170, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 64 terms through n = 2925 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"

/* Helper: compute 10^p for small p */
static seq_t pow10_u(idx_t p)
{
    seq_t r = 1;
    while (p--) r *= 10;
    return r;
}
/* a(1) = 0; a(2) = 4; a(3) = 8; a(4) = 44; … */

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 1) return 0;           /* first term */

    idx_t target = n;
    idx_t count  = 1;                /* count of terms found; 0 counted already */

    for (idx_t len = 1; ; ++len) {
        if (len == 1) {
            /* single digit numbers (1..9) */
            for (seq_t d = 1; d <= 9; ++d) {
                if (d % 4 == 0) {
                    ++count;
                    if (count == target) return d;
                }
            }
        } else if ((len & 1) == 0) { /* even length: 2k */
            idx_t k = len / 2;
            seq_t pow_k = pow10_u(k);
            seq_t start = pow10_u(k - 1);
            seq_t end   = pow10_u(k) - 1;
            seq_t i = start;
            while (i <= end) {
                seq_t rev = reverse_digits(i);
                seq_t pal = i * pow_k + rev;
                if (pal % 4 == 0) {
                    ++count;
                    if (count == target) return pal;
                }
                ++i;
            }
        } else { /* odd length: 2k+1 */
            idx_t k = (len - 1) / 2;
            seq_t pow_k    = pow10_u(k);
            seq_t pow_kp1  = pow10_u(k + 1);
            seq_t start = pow10_u(k - 1);
            seq_t end   = pow10_u(k) - 1;
            seq_t i = start;
            while (i <= end) {
                seq_t rev   = reverse_digits(i);
                seq_t base  = i * pow_kp1 + rev;   /* left part + right part (without middle) */
                seq_t d = 0;
                while (d <= 9) {
                    seq_t pal = base + d * pow_k;  /* insert middle digit */
                    if (pal % 4 == 0) {
                        ++count;
                        if (count == target) return pal;
                    }
                    ++d;
                }
                ++i;
            }
        }
    }

    /* unreachable */
    return 0;
}
