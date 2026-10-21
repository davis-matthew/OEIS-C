/*
 * A089370  Begin with n and place (n-1) on the most significant side, place
 *          (n-2) on the least significant side and so on until a 1 is placed.
 * https://oeis.org/A089370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   pow10_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089370 – Construct each term by alternately placing the next
 *  smaller integer on the left (most‑significant side) and the right
 *  (least‑significant side), beginning with n and proceeding down to 1.
 */
#include <stdbool.h>
#include "../utils/pow10_small.h"

/* Return 10^e as a seq_t (safe for small e). */

/* Return the decimal digit length of a seq_t value. */
static int digit_len(seq_t x)
{
    int len = 0;
    do {
        x /= 10;
        ++len;
    } while (x > 0);
    return len;
}

/* Compute A089370 */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety, offset is 1 in OEIS */
        return 0;

    seq_t val = (seq_t)n;                 /* starting value = n            */
    int d_val = digit_len(val);           /* current number of digits       */
    seq_t pow10_val = pow10_small(d_val); /* 10^digits(val)                 */
    bool left = true;                     /* next placement is on the left */

    for (idx_t i = n - 1; i > 0; --i)
    {
        seq_t i_seq = (seq_t)i;
        int di = digit_len(i_seq);
        seq_t pow10_di = pow10_small(di);

        if (left) {                      /* prepend i_seq to the left side */
            val = i_seq * pow10_val + val;
        } else {                         /* append i_seq to the right side */
            val = val * pow10_di + i_seq;
        }

        left = !left;                     /* alternate sides */

        /* Update state for the next iteration */
        pow10_val = pow10_val * pow10_di; /* 10^(current digits) */
        d_val     += di;                  /* digits increase by di   */
    }

    return val;
}
