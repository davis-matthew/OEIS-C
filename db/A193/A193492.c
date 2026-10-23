/*
 * A193492  Put the natural numbers together without spaces and read them four
 *          at a time advancing one space each time.
 * https://oeis.org/A193492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193492:
   a(n) is the 4‑digit number formed by the digits in positions n, n+1, n+2, n+3
   of the infinite string obtained by concatenating the natural numbers
   1,2,3,4,5,... with no delimiters. */
#include <stdint.h>

/* Pre‑compute powers of ten up to 10^20; the sequence values never
   require more than 20 decimal places for practical n. */
static seq_t pow10[21];
static int pow10_inited = 0;
static void init_pow10(void)
{
    if (pow10_inited) return;
    pow10[0] = 1;
    for (int i = 1; i <= 20; ++i)
        pow10[i] = pow10[i-1] * 10;
    pow10_inited = 1;
}

/* Return the decimal digit (0..9) at global position 'pos' (1‑based) of
   the concatenated natural‑number string. */
static seq_t digit_at(idx_t pos)
{
    init_pow10();
    idx_t remaining = pos;

    for (int len = 1; ; ++len) {
        seq_t       start  = (len == 1) ? (seq_t)1 : pow10[len-1];
        seq_t       count  = (len == 1) ? (seq_t)9 : (seq_t)9 * pow10[len-1];
        __uint128_t block  = (__uint128_t)count * (unsigned)len;

        if ((__uint128_t)remaining <= block) {
            /* Found the block containing the position */
            idx_t number_index = (remaining - 1u) / (unsigned)len;
            idx_t digit_index  = (remaining - 1u) % (unsigned)len;
            seq_t number       = start + (seq_t)number_index;
            int  di            = (int)digit_index;          // 0‑based
            seq_t pow10d       = pow10[len - 1 - di];
            seq_t digit        = (number / pow10d) % (seq_t)10;
            return digit;
        } else {
            remaining -= (idx_t)block;
        }
    }
}

/* Main function required by the sequence.  It can be called in any order
   and returns the value of a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t d0 = digit_at(n);
    seq_t d1 = digit_at(n + 1);
    seq_t d2 = digit_at(n + 2);
    seq_t d3 = digit_at(n + 3);

    return d0 * (seq_t)1000 + d1 * (seq_t)100 + d2 * (seq_t)10 + d3;
}
