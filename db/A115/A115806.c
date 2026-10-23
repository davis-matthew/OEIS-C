/*
 * A115806  Sequence A115805 in binary.
 * https://oeis.org/A115806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115806:
   a(0) = 0.
   For k = 0,1,2,… produce:
       t0 = 11 · 10^k
       if k ≥ 6:
           for i = 0 .. k‑6:
               ti = 11 · 10^k + 11 · 10^i
   The terms t0, t1, … (in this order) form the sequence. */
#include <stdbool.h>

/* Compute 10^exp as a seq_t, avoiding undefined shift behaviour. */
static seq_t pow10_idx(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find the block k containing the n-th term (1‑based index). */
    idx_t k = 0;
    idx_t cumulative = 0;
    while (1)
    {
        idx_t block_len = (k < 6) ? 1 : k - 4;
        if (cumulative + block_len >= n)
            break;
        cumulative += block_len;
        ++k;
    }

    /* 0‑based position inside the block. */
    idx_t pos = n - cumulative - 1;

    seq_t base = (seq_t)11 * pow10_idx(k);
    if (pos == 0)
        return base;

    /* In the addition part pos-1 corresponds to i. */
    idx_t i = pos - 1;
    seq_t add = (seq_t)11 * pow10_idx(i);
    return base + add;
}
