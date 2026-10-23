/*
 * A182274  Starting with 1, the smallest number not yet in the sequence such
 *          that the sum of every pair of adjacent digits in the sequence sum
 *          to a square number.
 * https://oeis.org/A182274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest unused integer such that the concatenation of
 *            all sequence terms has the property that every pair of adjacent
 *            decimal digits sums to a perfect square (0,1,4,9,16). The first
 *            term is 1
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   square_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/square_digit_sum.h"

seq_t a(idx_t n)
{
    /* Static storage for the sequence and used numbers. */
    static seq_t *seq      = NULL;   /* sequence values */
    static seq_t *used     = NULL;   /* used test values */
    static size_t seq_cap  = 0;      /* capacity of seq/used */
    static size_t seq_len  = 0;      /* number of terms already stored */
    static int    last_digit = -1;   /* last decimal digit of the last term */

    /* Ensure we have at least the first term initialized. */
    if (seq_len == 0) {
        seq_cap = 64;
        seq    = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        used   = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        used[0] = seq[0];
        seq_len  = 1;
        last_digit = 1;           /* last digit of the first term */
    }

    /* Convert idx_t to a native type for the loop bounds. */
    size_t target = (size_t)n;      /* n is guaranteed >= 1 */

    /* Grow the sequence until we have computed up to index target. */
    while (seq_len < target) {
        if (seq_len >= seq_cap) {
            seq_cap *= 2;
            seq   = (seq_t *)realloc(seq,   seq_cap * sizeof(seq_t));
            used  = (seq_t *)realloc(used,  seq_cap * sizeof(seq_t));
        }

        /* Search for the next term, starting from 1 upward. */
        seq_t candidate = 1;
        for (;;) {
            /* Skip numbers already used. */
            bool used_flag = false;
            for (size_t i = 0; i < seq_len; ++i) {
                if (used[i] == candidate) {
                    used_flag = true;
                    break;
                }
            }
            if (used_flag) {
                ++candidate;
                continue;
            }

            /* Extract decimal digits of candidate into array digits[high..low]. */
            seq_t tmp = candidate;
            uint8_t digits[40];   /* 128‑bit decimal can have at most 39 digits. */
            size_t  dlen = 0;
            if (tmp == 0) {
                digits[0] = 0;
                dlen = 1;
            } else {
                while (tmp > 0) {
                    digits[dlen++] = (uint8_t)(tmp % 10);
                    tmp /= 10;
                }
            }

            /* Test internal adjacency of digits. */
            bool ok = true;
            for (size_t i = dlen; i > 1; --i) {
                int a = digits[i - 1];
                int b = digits[i - 2];
                if (!square_digit_sum(a, b)) {
                    ok = false;
                    break;
                }
            }

            /* Test the junction with the previous term, if any. */
            if (ok && last_digit != -1) {
                int s = last_digit + digits[dlen - 1];   /* first (most significant) digit */
                if (!square_digit_sum(last_digit, digits[dlen - 1])) {
                    ok = false;
                }
            }

            if (ok) {
                /* Found the next term. Store it. */
                seq[seq_len] = candidate;
                used[seq_len] = candidate;
                ++seq_len;
                last_digit = digits[0];   /* least significant digit */
                break;
            } else {
                ++candidate;               /* try the next integer */
            }
        }
    }

    return seq[target - 1];
}
