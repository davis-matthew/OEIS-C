/*
 * A037357  Numbers whose base-4 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A037357 – numbers whose base‑4 and base‑10 representations share
 *            no digits. Computed by brute‑force search
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool good_candidate(seq_t num)
{
    unsigned int dec_bits = 0;              /* bitmask of decimal digits 0–3 present */
    seq_t t = num;

    /* Gather decimal digits that are < 4 */
    while (t > (seq_t)0) {
        unsigned int d = (unsigned int)(t % (seq_t)10);
        if (d < 4u) dec_bits |= 1u << d;
        t /= (seq_t)10;
    }

    /* Scan base‑4 digits, reject if any overlaps with the decimal set */
    t = num;
    while (t > (seq_t)0) {
        unsigned int d = (unsigned int)(t % (seq_t)4);
        if (dec_bits & (1u << d)) return false;
        t /= (seq_t)4;
    }

    return true;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1).  For n == 0 the sequence is undefined, return 0. */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t num = (seq_t)1;
    idx_t count = (idx_t)0;

    while (1) {
        if (good_candidate(num)) {
            ++count;
            if (count == n) return num;
        }
        ++num;
    }
}
