/*
 * A165074  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-7 Kaprekar map in A165071.
 * https://oeis.org/A165074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (length of the preperiodic part) + (length of the cycle)
 *            of the trajectory of n under the base‑7 Kaprekar map
 * Range:     u8 n <= 54, u16 n <= 2401, u32 n <= 2401, u64 n <= 2401,
 *            u128 n <= 2401, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t kaprekar_next(seq_t n) {
    /* convert n to base‑7 digits (least‑significant first) */
    seq_t tmp = n;
    uint8_t digits[32];
    idx_t len = 0;
    if (tmp == (seq_t)0) {
        digits[len++] = 0;
    } else {
        while (tmp != (seq_t)0) {
            digits[len++] = (uint8_t)(tmp % (seq_t)7);
            tmp     = tmp / (seq_t)7;
        }
    }

    /* sort digits ascending */
    for (idx_t i = 0; i < len; ++i) {
        for (idx_t j = i + 1; j < len; ++j) {
            if (digits[j] < digits[i]) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }

    /* compute min (ascending order) */
    seq_t min_val = (seq_t)0;
    for (idx_t i = 0; i < len; ++i) {
        min_val = min_val * (seq_t)7 + (seq_t)digits[i];
    }

    /* compute max (descending order) */
    seq_t max_val = (seq_t)0;
    for (idx_t i = len; i > 0; --i) {
        max_val = max_val * (seq_t)7 + (seq_t)digits[i - 1];
    }

    return max_val - min_val;
}

seq_t a(idx_t n) {
    seq_t start = (seq_t)n;

    /* Floyd's cycle‑finding algorithm */
    seq_t tortoise = kaprekar_next(start);
    seq_t hare     = kaprekar_next(kaprekar_next(start));
    while (tortoise != hare) {
        tortoise = kaprekar_next(tortoise);
        hare     = kaprekar_next(kaprekar_next(hare));
    }

    /* find preperiodic length (mu) */
    idx_t mu = 0;
    seq_t mu_tort = start;
    seq_t mu_hare = tortoise;
    while (mu_tort != mu_hare) {
        mu_tort = kaprekar_next(mu_tort);
        mu_hare = kaprekar_next(mu_hare);
        ++mu;
    }

    /* find cycle length (lambda) */
    idx_t lam = 1;
    seq_t lam_next = kaprekar_next(mu_tort);
    while (lam_next != mu_tort) {
        lam_next = kaprekar_next(lam_next);
        ++lam;
    }

    return (seq_t)mu + (seq_t)lam;
}
