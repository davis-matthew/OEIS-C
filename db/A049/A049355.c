/*
 * A049355  Digitally balanced numbers in base 4: equal numbers of 0's, 1's,
 *          ... 3's.
 * https://oeis.org/A049355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 59 terms through n = 883 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: Digitally balanced numbers in base 4.
   a(n) is the nth positive integer whose base‑4 representation
   contains each of the digits 0,1,2,3 exactly the same number of times.
   We compute a(n) by scanning numbers in increasing order and testing this property.
*/
#include <stdint.h>
#include <stdbool.h>

static bool is_balanced_uint64(uint64_t v)
{
    /* Count each base‑4 digit (two least significant bits) */
    if (v == 0) return false;          /* 0 is not balanced */
    uint8_t cnt[4] = {0,0,0,0};
    while (v) {
        cnt[v & 3]++;                   /* digit v mod 4 */
        v >>= 2;                       /* divide by 4 */
    }
    return (cnt[0] == cnt[1] &&
            cnt[1] == cnt[2] &&
            cnt[2] == cnt[3]);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not used (offset 1) */

    idx_t found = 0;
    uint64_t x = 1;                   /* start from the first positive integer */

    for (;;) {
        if (is_balanced_uint64(x)) {
            ++found;
            if (found == n)
                return (seq_t)x;
        }
        ++x;
    }
}
