/*
 * A057891  In base 2, neither a palindrome nor becomes a palindrome if
 *          trailing 0's are omitted.
 * https://oeis.org/A057891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057891: numbers whose binary representation is neither a palindrome
   nor becomes a palindrome after removing all trailing zeros. */

#include <stdbool.h>

bool is_bin_pal(seq_t x) {
    if (x == 0) return true;           /* 0 is trivially a palindrome */
    unsigned len = 0;
    seq_t tmp = x;
    while (tmp) { tmp >>= 1; ++len; }  /* bit length */

    for (unsigned i = 0; i < len / 2; ++i) {
        unsigned lbit = (unsigned)((x >> i) & 1);
        unsigned rbit = (unsigned)((x >> (len - 1 - i)) & 1);
        if (lbit != rbit) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n=0, return 0 */

    idx_t count = 0;
    seq_t cand = 1;
    while (true) {
        /* test original binary string */
        if (!is_bin_pal(cand)) {
            /* strip all trailing zeros */
            seq_t tr = cand;
            while ((tr & 1) == 0) tr >>= 1;
            if (!is_bin_pal(tr)) {
                ++count;
                if (count == n) return cand;
            }
        }
        ++cand;
    }
}
