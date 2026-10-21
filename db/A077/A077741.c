/*
 * A077741  Smallest multiple of n which begins with R(n) and ends in n where
 *          R(n) (A004086) is the digit reversal of n. Suitable number of
 *          zeros are assumed to the left of the MSD if required.
 * https://oeis.org/A077741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer m such that * 1) m is a
 *            multiple of n, * 2) the decimal representation of m, after *
 *            padding with arbitrary leading zeros if necessary, * starts with
 *            rev(n) (the digit reversal of n), * and ends with n. * The
 *            sequence is indexed starting at 1
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_len, digits, gcd, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/digit_len.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* irrelevant, offset starts at 1 */

    seq_t N = (seq_t)n;

    /* number of decimal digits of n */
    int len_rev = digits((uint64_t)n);            /* same as digit_len(N) */

    /* rev(n) written with exactly len_rev digits */
    unsigned char revbuf[64] = {0};
    uint64_t tmp = n;
    for (int i = 0; i < len_rev; ++i) {
        revbuf[i] = (unsigned char)(tmp % 10);    /* most significant digit first */
        tmp /= 10;
    }

    /* how many leading zeros are in rev(n) */
    int zero_prefix = 0;
    while (zero_prefix < len_rev && revbuf[zero_prefix] == 0) {
        ++zero_prefix;
    }

    /* arithmetic progression step that preserves the suffix condition */
    seq_t base   = pow10((idx_t)len_rev);          /* 10^len_rev */
    seq_t g      = gcd(N, base);
    seq_t step_prime = base / g;                   /* integer division */
    seq_t step  = seq_mul(N, step_prime);          /* n * step_prime */

    seq_t m = N;                                   /* first candidate */

    for (;;) {
        /* suffix automatically satisfied by construction (m ≡ n mod 10^len_rev) */

        int len_m = digit_len(m);
        int rem_len = len_rev - zero_prefix;      /* digits of rev(n) that must come from m */

        if (len_m >= rem_len) {
            /* divisor of the most significant digit of m */
            seq_t divisor = pow10((idx_t)len_m - 1);
            bool ok = true;
            int j = 0;
            for (; j < len_rev; ++j) {
                unsigned char expected = revbuf[j];
                if (j < zero_prefix) {
                    /* leading zeros are satisfied by padding */
                    if (expected != 0) { ok = false; break; }
                    continue;
                }
                /*** m digit at position j - zero_prefix ***/
                seq_t digit = (m / divisor) % (seq_t)10;
                if ((unsigned char)digit != expected) { ok = false; break; }
                divisor /= 10;
            }
            if (ok) return m;
        }
        m += step;
    }
}
