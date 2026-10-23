/*
 * A046330  Palindromes with exactly 4 prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A046330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromes with exactly 4 prime factors (counted with multiplicity) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse.h"
#include "../utils/pow10.h"
#include "../utils/bigomega.h"

/* Helper to compute 10^k as idx_t (k >= 0) */
static idx_t pow10_idx(int k)
{
    idx_t res = 1;
    for (int i = 0; i < k; ++i) res *= 10;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, harmless */

    idx_t found = 0;

    /* 1‑digit palindromes */
    for (int d = 1; d <= 9; ++d) {
        seq_t p = (seq_t)d;
        if (bigomega((idx_t)p) == 4) {
            ++found;
            if (found == n) return p;
        }
    }

    /* palindromes of length >= 2 */
    for (int len = 2; ; ++len) {
        if (len % 2 == 0) {                 /* even length */
            int half = len / 2;
            idx_t start = pow10_idx(half - 1);          /* first digit non‑zero */
            idx_t end   = pow10_idx(half) - 1;           /* last half with all 9s */
            seq_t pow_half = pow10((idx_t)half);        /* 10^half as seq_t */

            for (idx_t left = start; left <= end; ++left) {
                seq_t left_seq = (seq_t)left;
                seq_t rev = reverse(left_seq);
                seq_t p = left_seq * pow_half + rev;
                if (bigomega((idx_t)p) == 4) {
                    ++found;
                    if (found == n) return p;
                }
            }
        } else {                             /* odd length */
            int half = len / 2;                /* floor(len/2) */
            idx_t start = pow10_idx(half - 1);
            idx_t end   = pow10_idx(half) - 1;
            seq_t pow_half = pow10((idx_t)half);           /* 10^half */
            seq_t pow_half_plus1 = pow10((idx_t)(half + 1)); /* 10^(half+1) */

            for (idx_t left = start; left <= end; ++left) {
                seq_t left_seq = (seq_t)left;
                seq_t rev = reverse(left_seq);
                for (int mid = 0; mid <= 9; ++mid) {
                    seq_t p = left_seq * pow_half_plus1
                               + (seq_t)mid * pow_half
                               + rev;
                    if (bigomega((idx_t)p) == 4) {
                        ++found;
                        if (found == n) return p;
                    }
                }
            }
        }
    }
}
