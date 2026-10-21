/*
 * A078453  Numbers in which all the digits are coprime to each other.
 * https://oeis.org/A078453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 2756, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078453: Numbers in which all the digits are pairwise coprime.
   a(1) = 0, a(2) = 1, a(3) = 2, …   */
#include <stdbool.h>
#include "../utils/gcd.h"

/* Return true iff all decimal digits of x are pairwise coprime.
   The digit 1 is coprime with everything and may appear repeatedly.
   The digit 0 may appear only together with digits 1 (no other
   non‑1 digit may occur) and may appear at most once, because gcd(0,0)=0. */
static bool valid_candidate(idx_t x)
{
    if (x == 0)          /* 0 has no digits but is considered valid */
        return true;

    bool seen[10] = {false};
    bool zero_seen = false;
    uint8_t non1[10];
    int non1_cnt = 0;

    idx_t t = x;
    while (t > 0) {
        uint8_t d = (uint8_t)(t % 10);
        t /= 10;

        if (seen[d]) {
            /* repeated digit */
            if (d == 1)          /* 1 may repeat */
                continue;
            else                 /* any other digit cannot repeat */
                return false;
        }
        seen[d] = true;

        if (d == 0) {
            if (zero_seen)       /* zero cannot repeat */
                return false;
            zero_seen = true;
            /* 0 may appear together with 1's only, so we postpone
               the final check until all digits have been examined. */
            continue;
        }

        if (d != 1) {
            non1[non1_cnt++] = d;
            if (non1_cnt > 4)    /* at most four distinct non‑1 digits allowed */
                return false;
        }
    }

    /* A zero digit may appear only when no non‑1 digits are present. */
    if (zero_seen && non1_cnt > 0)
        return false;

    /* Check pairwise coprimality of the distinct non‑1 digits. */
    for (int i = 0; i < non1_cnt; ++i) {
        for (int j = i + 1; j < non1_cnt; ++j) {
            if (gcd((seq_t)non1[i], (seq_t)non1[j]) != 1)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t seen = 0;
    idx_t candidate = 0;

    while (true) {
        if (valid_candidate(candidate)) {
            ++seen;
            if (seen == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
