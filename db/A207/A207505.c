/*
 * A207505  Start with n, successively subtract the next digit of the
 *          resulting sequence, stop when reaching zero or less: a(n) is the
 *          absolute value of the result.
 * https://oeis.org/A207505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3021, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207505 – Digit‑trail "miss numbers"
 *
 *  Computes a(n) by simulating the following process:
 *  - Start with the integer n.
 *  - Consider the decimal digits of all numbers produced so far
 *    in a left‑to‑right queue.  Initially this queue consists
 *    of the digits of n.
 *  - While the current value v is positive:
 *        • Take the next digit d from the queue.
 *        • If v ≤ d then the process stops and the result is
 *          abs(v – d)  (which is d – v).
 *        • Otherwise set v = v – d and append the decimal digits
 *          of the new v to the end of the queue.
 *  - The function returns the absolute value of the final result.
 *
 *  This matches the definition of OEIS A207505 (miss numbers).
 *
 *  The implementation is O(T) where T is the number of digits
 *  processed during the run, and it uses only integer arithmetic.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* working value */
    seq_t v = (seq_t)n;

    /* queue of digits (most‑significant first) */
    size_t cap = 64;
    size_t len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) {
        /* allocation failure – return 0 as a fallback */
        return (seq_t)0;
    }

    /* initialise queue with digits of n */
    size_t ndigs = digit_len(v);
    seq_t div = pow10((idx_t)(ndigs - 1));
    for (size_t i = 0; i < ndigs; ++i) {
        uint8_t d = (uint8_t)((v / div) % (seq_t)10);
        digits[len++] = d;
        div /= (seq_t)10;
    }

    size_t idx = 0;   /* next digit to consume */

    while (v > (seq_t)0) {
        uint8_t d = digits[idx++];
        if (v <= (seq_t)d) {
            /* v – d ≤ 0, so final result is d – v */
            v = (seq_t)d - v;
            break;
        } else {
            v -= (seq_t)d;   /* v > d, so v remains positive */

            /* append digits of the new v to the queue */
            size_t ndigs2 = digit_len(v);
            seq_t div2 = pow10((idx_t)(ndigs2 - 1));

            for (size_t k = 0; k < ndigs2; ++k) {
                uint8_t d2 = (uint8_t)((v / div2) % (seq_t)10);

                /* grow the queue if necessary */
                if (len + 1 > cap) {
                    size_t new_cap = cap * 2;
                    uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
                    if (!tmp) {
                        free(digits);
                        return (seq_t)0;
                    }
                    digits = tmp;
                    cap = new_cap;
                }
                digits[len++] = d2;
                div2 /= (seq_t)10;
            }
        }
    }

    free(digits);
    return v;   /* v is already non‑negative (abs value) */
}
