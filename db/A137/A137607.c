/*
 * A137607  a(0)=361, a(n+1)=a(n)/2 if a(n) is even, else
 *          a(n+1)=a(0)-(a(n)+1)/2.
 * https://oeis.org/A137607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 102, u32 n <= 102, u64 n <= 102, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 361
   For n ≥ 0:
     if a(n) is even, a(n+1) = a(n)/2;
     else a(n+1) = a(0) - (a(n)+1)/2.
   The sequence is periodic for n ≥ 1 with period 51. */

#include <stdbool.h>

static bool seq_initialized = false;
static seq_t a_vals[52];    /* a_vals[0] = a(0), a_vals[1..51] = a(1..51) */

static void init_seq(void)
{
    if (seq_initialized)
        return;

    a_vals[0] = (seq_t)361;
    seq_t cur = a_vals[0];

    for (idx_t i = 1; i <= 51; ++i) {
        if ((cur & (seq_t)1) == 0)          /* even */
            cur = cur >> 1;                 /* cur / 2 */
        else                                /* odd */
            cur = a_vals[0] - (cur + (seq_t)1) / (seq_t)2;
        a_vals[i] = cur;
    }
    seq_initialized = true;
}

seq_t a(idx_t n)
{
    init_seq();

    if (n == 0)
        return a_vals[0];

    idx_t idx = ((n - 1) % 51) + 1;
    return a_vals[idx];
}
