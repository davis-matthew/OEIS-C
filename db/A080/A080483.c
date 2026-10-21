/*
 * A080483  Reverse concatenation of next n numbers with a(1) = 0.
 * https://oeis.org/A080483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse concatenation of next n numbers.
   For n ≥ 1: a(n) = concat( n(n+1)/2‑1 , n(n+1)/2‑2 , … , n(n+1)/2‑n )
   Example: a(5) = 14 13 12 11 10 = 1413121110   */

#include <stdint.h>
#include <stdbool.h>

/* number of decimal digits in an unsigned 64‑bit integer */
static inline int digits_uint64(uint64_t x)
{
    int d = 1;
    while (x >= 10) {
        x /= 10;
        d++;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* compute the starting integer of the block */
    seq_t nn   = (seq_t)n;
    seq_t start = nn * (nn + 1) / 2 - 1;   /* n(n+1)/2 – 1 */

    /* pre‑computed powers of ten (up to 10 digits) */
    static seq_t pow10[11];
    static bool init = false;
    if (!init) {
        pow10[0] = 1;
        for (int i = 1; i <= 10; ++i) {
            pow10[i] = pow10[i - 1] * 10;
        }
        init = true;
    }

    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t cur = start - (seq_t)i;
        /* current number has at most 10 decimal digits for n ≤ 10000 */
        uint64_t tmp = (uint64_t)cur;
        int d = digits_uint64(tmp);
        result = result * pow10[d] + cur;
    }
    return result;
}
