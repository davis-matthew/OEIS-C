/*
 * A037394  Numbers k such that every base-5 digit of k is a base-7 digit of
 *          k.
 * https://oeis.org/A037394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 8187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037394: Numbers k such that every base‑5 digit of k
   is also a digit appearing in the base‑7 representation of k.
   The function a(n) is computed by a linear search. */

#include <stdbool.h>

static bool is_good(seq_t k)
{
    uint8_t mask5 = 0, mask7 = 0;
    seq_t t = k;

    /* Mask of digits in base‑5 */
    while (t > 0) {
        uint8_t d = (uint8_t)(t % 5);
        mask5 |= 1U << d;
        t /= 5;
    }

    t = k;
    /* Mask of digits in base‑7 */
    while (t > 0) {
        uint8_t d = (uint8_t)(t % 7);
        mask7 |= 1U << d;
        t /= 7;
    }

    /* All digits from base‑5 must appear in base‑7 */
    return (mask5 & ~mask7) == 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, but harmless */

    idx_t count = 0;
    seq_t cur = 1;

    while (1) {
        if (is_good(cur)) {
            count++;
            if (count == n) return cur;
        }
        cur++;
    }
}
