/*
 * A053063  Pendulum numbers: alternately append n to beginning or end of
 *          previous term.
 * https://oeis.org/A053063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pendulum numbers: a(1)=1; for n>1,
   append n at the end if n is odd, prepend if even.
   Recurrence:
   a(n) = n * 10^{len(a(n‑1))} + a(n‑1)   (n even)
   a(n) = a(n‑1) * 10^{len(n)} + n        (n odd)
*/
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/mul_small.h"

static inline size_t digit_len(idx_t x)
{
    size_t len = 0;
    do { len++; x /= 10; } while (x);
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    seq_t result = 1;              /* a(1) */
    if (n == 1) return result;

    static seq_t pow10_small[6];
    static bool pow10_small_init = false;
    if (!pow10_small_init) {
        for (int k = 0; k <= 5; ++k)
            pow10_small[k] = pow10((idx_t)k);
        pow10_small_init = true;
    }

    size_t len_total = 1;                  /* digits in result so far */
    seq_t ten_pow_len_total = pow10_small[1]; /* 10^len_total */

    for (idx_t i = 2; i <= n; ++i) {
        size_t len_i = digit_len(i);
        seq_t small_pow = pow10_small[(int)len_i];

        if ((i & 1) == 0) { /* even: prepend */
            seq_t term = mul_small(ten_pow_len_total, (unsigned int)i);
            result += term;
        } else {            /* odd: append */
            result = result * small_pow + (seq_t)i;
        }

        /* update for next iteration */
        len_total += len_i;
        ten_pow_len_total = ten_pow_len_total * small_pow;
    }

    return result;
}
