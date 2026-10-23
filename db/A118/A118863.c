/*
 * A118863  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 16.
 * https://oeis.org/A118863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digit_len, pow10, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1
   a(n) = rotate_first_digit(a(n-1)) + 16  for n > 1
   rotate_first_digit(x):  move the most significant decimal digit of x to the end
   (e.g., 123 → 231).  After the first step the sequence becomes periodic of
   period 18. */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/seq_one.h"

static inline seq_t rotate_first_digit(seq_t x)
{
    int len = digit_len(x);
    if (len <= 1)
        return x;
    seq_t pow10_val = pow10((idx_t)(len - 1));
    seq_t msd  = x / pow10_val;          /* most significant digit   */
    seq_t rest = x % pow10_val;          /* remaining digits          */
    return rest * 10 + msd;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return seq_one();              /* a(1) = 1                     */

    static seq_t cycle[18];
    static bool init = false;

    if (!init) {
        seq_t v = seq_one();           /* start from a(1) = 1          */
        for (int i = 0; i < 18; ++i) {
            v = rotate_first_digit(v) + (seq_t)16;  /* a(n) for n=2..19   */
            cycle[i] = v;              /* store cycle[0] = a(2)       */
        }
        init = true;
    }

    idx_t idx = (n - 2) % 18;           /* n ≥ 2  → idx ∈ {0,…,17}      */
    return cycle[(int)idx];
}
