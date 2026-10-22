/*
 * A000861  Numbers ending with a vowel in American English.
 * https://oeis.org/A000861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, easy
 * Formula:   a(1) = 0; for n >= 2, with m = n-2, a(n) = 100*floor(m/46) + r(m
 *            mod 46), where r(0..45) lists in increasing order the 46
 *            residues t in 1..99 whose English name ends in a vowel (t mod 10
 *            in {1,2,3,5,9} and t not in 11..19, or t = 12). Equivalent to
 *            a(n) = a(n-1) + a(n-46) - a(n-47) for n > 48
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ends_with_vowel_mod100
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ends_with_vowel_mod100.h"

seq_t a(idx_t n) {
    unsigned res[46];
    int cnt = 0;
    unsigned t;
    idx_t m, q;
    int r;
    if (n <= 1) return (seq_t)0;
    for (t = 1; t < 100; t++)
        if (ends_with_vowel_mod100(t)) res[cnt++] = t;
    m = n - 2;
    q = m / 46;
    r = (int)(m % 46);
    return (seq_t)q * 100 + (seq_t)res[r];
}
