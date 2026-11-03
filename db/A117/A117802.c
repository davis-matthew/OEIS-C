/*
 * A117802  Numbers with an "a" in Dutch.
 * https://oeis.org/A117802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   numbers whose Dutch name contains the letter 'a'
 * Range:     u8 n <= 44, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* true if the number (0 < x < 1000) in Dutch contains the letter 'a' */
static bool part_has_a_small(uint32_t x)
{
    if (x == 0) return false;
    if (x < 20) return (x == 8 || x == 12 || x == 18);
    if (x < 100) {
        /* tens 80-89 are all 'tachtig' which contains 'a' */
        if (x >= 80 && x < 90) return true;
        /* otherwise only a unit of 8 gives 'a' */
        return (x % 10 == 8);
    }
    uint32_t hd  = x / 100;
    uint32_t rem = x % 100;
    if (hd == 8) return true;          /* 'acht' in the hundreds place */
    return part_has_a_small(rem);       /* check the remainder */
}

/* 1: duizend, 2: miljoen, 3: miljard, 4: biljoen, 5: biljard, ... */
static bool scale_has_a(idx_t scale)
{
    if (scale < 3) return false;          /* duizend, miljoen contain no 'a' */
    return (scale & 1ULL);                /* odd scales from 3 on ('jard') */
}

/* true if the Dutch name of n contains the letter 'a' */
static bool contains_a_dutch(seq_t n)
{
    seq_t  temp  = n;
    idx_t  scale = 0;
    while (temp > 0) {
        seq_t group = temp % (seq_t)1000;
        if (group > 0) {
            if (part_has_a_small((uint32_t)group)) return true;
            if (scale_has_a(scale)) return true;
        }
        temp /= (seq_t)1000;
        scale++;
    }
    return false;
}

/* true if the scale (thousands, million, etc.) name contains 'a' */

/* Return the n-th term of A117802 (n >= 1) */
seq_t a(idx_t n)
{
    idx_t count    = 0;
    seq_t candidate = 1;
    while (1) {
        if (contains_a_dutch(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
