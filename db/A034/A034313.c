/*
 * A034313  Decimal part of a(n)^(1/7) starts with reversal of its integer
 *          part: first term of runs.
 * https://oeis.org/A034313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   The index t of A034313 is the first integer m such that the
 *            decimal part of m^(1/7) begins with the reverse of its integer
 *            part. This is computed by iterating over the possible integer
 *            parts I, determining the consecutive m that satisfy the
 *            condition for that I, and merging consecutive runs. The...
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digits, rev, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/digits.h"
#include "../utils/rev.h"

/* Helper: 10^e; defined only for the unsigned __int128 branch. */
static unsigned __int128 pow10_u128(idx_t e)
{
    unsigned __int128 r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Current run boundaries.  They are only valid after the first run. */
    seq_t cur_low = 0, cur_high = 0;
    /* run_index == (idx_t)-1 until the first run is found. */
    idx_t run_index = (idx_t)-1;

    for (idx_t I = 1;; ++I)
    {
        /* Digits of the integer part I, used for the length of the
           reversed integer part. */
        idx_t d   = digits(I);
        idx_t revI = rev(I);

        if (sizeof(seq_t) <= sizeof(uint64_t))
        {
            /* 128‑bit arithmetic for small seq_t. */
            unsigned __int128 ten_d  = pow10_u128(d);
            unsigned __int128 ten_7d = pow10_u128(7 * d);

            unsigned __int128 A = (unsigned __int128)I * ten_d + revI;

            unsigned __int128 powA      = 1;
            unsigned __int128 powAplus  = 1;
            for (int i = 0; i < 7; ++i) {
                powA     *= A;
                powAplus *= (A + 1);
            }

            seq_t low  = (seq_t)((powA + ten_7d - 1) / ten_7d); /* ceil */
            seq_t high = (seq_t)((powAplus - 1) / ten_7d);       /* floor */

            /* Merge or start a new run. */
            if (low <= high)
            {
                if (run_index == (idx_t)-1 || low > cur_high + 1)
                {
                    ++run_index;
                    cur_low  = low;
                    cur_high = high;
                }
                else
                {
                    if (high > cur_high) cur_high = high;
                }
                if (run_index == n) return cur_low;
            }
        }
        else
        {
            /* Arbitrary‑precision arithmetic. */
            seq_t ten_d  = seq_pow((seq_t)10, d);
            seq_t ten_7d = seq_pow((seq_t)10, 7 * d);

            seq_t A = (seq_t)I * ten_d + (seq_t)revI;

            seq_t powA     = seq_pow(A,             7);
            seq_t powAplus = seq_pow(A + (seq_t)1,  7);

            seq_t low  = (powA + ten_7d - (seq_t)1) / ten_7d;
            seq_t high = (powAplus - (seq_t)1)       / ten_7d;

            if (low <= high)
            {
                if (run_index == (idx_t)-1 || low > cur_high + (seq_t)1)
                {
                    ++run_index;
                    cur_low  = low;
                    cur_high = high;
                }
                else
                {
                    if (high > cur_high) cur_high = high;
                }
                if (run_index == n) return cur_low;
            }
        }
    }
}
