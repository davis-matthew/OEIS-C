/*
 * A209791  Half the number of (n+1)X4 0..2 arrays with every 2X2 subblock
 *          having exactly one duplicate clockwise edge difference.
 * https://oeis.org/A209791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A209791.
   a(n) = 23*a(n-1) +474*a(n-2) -947*a(n-3) -13207*a(n-4)
          +27207*a(n-5) +14500*a(n-6) -33255*a(n-7) -1722*a(n-8)
          +5600*a(n-9) +816*a(n-10)
   Initial values (n = 1..10):
   1380, 49056, 1732554, 61289190, 2167312950,
   76648760118, 2710658447034, 95862567834618,
   3390172069647198, 119893318115163858.
   This recurrence has been verified for the terms in the OEIS
   sequence.  It is used to compute a(n) for any n ≥ 1.  The
   implementation stores only the previous 10 terms, so the
   memory usage is constant.  All arithmetic is performed using
   the type seq_t, which is either a built‑in unsigned integer
   or an arbitrary‑precision integer class provided by the
   surrounding framework.  */

#include <stddef.h>

static const seq_t init_vals[10] = {
    (seq_t)1380ULL,
    (seq_t)49056ULL,
    (seq_t)1732554ULL,
    (seq_t)61289190ULL,
    (seq_t)2167312950ULL,
    (seq_t)76648760118ULL,
    (seq_t)2710658447034ULL,
    (seq_t)95862567834618ULL,
    (seq_t)3390172069647198ULL,
    (seq_t)119893318115163858ULL
};

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    if (n <= 10) {
        return init_vals[n - 1];
    }

    seq_t prev[10];
    for (size_t i = 0; i < 10; ++i) {
        prev[i] = init_vals[i];
    }

    seq_t term = (seq_t)0;
    for (idx_t i = 11; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* a(n-1) .. a(n-10) are stored in prev[9] .. prev[0] */
        pos += (seq_t)23   * prev[9];
        pos += (seq_t)474  * prev[8];
        neg += (seq_t)947  * prev[7];
        neg += (seq_t)13207* prev[6];
        pos += (seq_t)27207* prev[5];
        pos += (seq_t)14500* prev[4];
        neg += (seq_t)33255* prev[3];
        neg += (seq_t)1722 * prev[2];
        pos += (seq_t)5600 * prev[1];
        pos += (seq_t)816  * prev[0];

        term = pos - neg;  /* unsigned subtraction; safe because
                               the result is known to be positive */

        /* shift the window: drop a(n-10) and add a(n) */
        for (size_t j = 0; j < 9; ++j) {
            prev[j] = prev[j + 1];
        }
        prev[9] = term;
    }

    return term;
}
