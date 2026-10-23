/*
 * A037363  Numbers whose base-6 and base-7 expansions have no digits in
 *          common.
 * https://oeis.org/A037363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037363: Numbers whose base‑6 and base‑7 expansions have no digits in common.
 *
 * This implementation enumerates integers, building bit masks of the
 * digits used in base‑6 and base‑7 forms.  A number is accepted when the
 * two masks have no common bits, i.e. the two sets of digits are disjoint.
 * The counter is compared with the requested index `n`; when equality
 * holds the current value is returned.  This approach runs in O(n·log n)
 * time, which is fast enough for the intended range (n ≈ 10 000) on all
 * supported builds.
 */

#include <stdbool.h>

static bool good(seq_t num)
{
    seq_t tmp = num;
    seq_t mask6 = 0;
    seq_t mask7 = 0;

    /* Base‑6 representation */
    while (tmp > 0) {
        seq_t d = tmp % 6;
        mask6 |= ((seq_t)1 << (unsigned)d);
        tmp /= 6;
    }

    tmp = num;
    /* Base‑7 representation */
    while (tmp > 0) {
        seq_t d = tmp % 7;
        mask7 |= ((seq_t)1 << (unsigned)d);
        tmp /= 7;
    }

    return (mask6 & mask7) == 0;
}

seq_t a(idx_t n)
{
    seq_t x = 1;
    idx_t count = 0;

    while (1) {
        if (good(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
