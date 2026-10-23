/*
 * A023494  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 2) and
 *          d(n) = (n-th non-Lucas number).
 * https://oeis.org/A023494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+4)+A090946(n+2) b(n) = n-th Fibonacci number >
 *            2 d(n) = n-th non‑Lucas number (Lucas numbers start 1,3,4,7,…)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/fib.h"

static seq_t lucas_buf[64];     /* small cache of Lucas numbers 1,3,4,7,... */
static size_t lucas_len = 0;    /* number of entries in lucas_buf */
static bool lucas_inited = false;

/* Initialise the cache with the first two Lucas numbers */
static void init_lucas(void)
{
    if (!lucas_inited) {
        lucas_buf[0] = (seq_t)1;    /* L0 = 1   */
        lucas_buf[1] = (seq_t)3;    /* L1 = 3   */
        lucas_len = 2;
        lucas_inited = true;
    }
}

/* Ensure lucas_buf contains all Lucas numbers ≤ x,
   and return true iff x is a Lucas number. */
static bool is_lucas(seq_t x)
{
    init_lucas();
    /* Grow the cache to cover x */
    while (lucas_len >= 2 && lucas_buf[lucas_len - 1] < x) {
        /* next Lucas = last + second last */
        if (lucas_len == sizeof(lucas_buf)/sizeof(lucas_buf[0]))
            break;                     /* should not happen for n ≤ 10000 */
        lucas_buf[lucas_len] = lucas_buf[lucas_len - 1] + lucas_buf[lucas_len - 2];
        ++lucas_len;
    }
    /* Linear search – the cache is tiny (≈2–3 elements for the needed range). */
    for (size_t i = 0; i < lucas_len; ++i)
        if (lucas_buf[i] == x)
            return true;
    return false;
}

/* Return the n‑th non‑Lucas number (offset 0).
   The sequence starts with 2,5,6,8,9,… */
static seq_t nonlucas_n(idx_t n)
{
    seq_t cand = (seq_t)1;           /* start just before the first non‑Lucas (which is 2) */
    idx_t count = 0;
    for (;;) {
        if (!is_lucas(cand)) {
            if (count == n)
                return cand;
            ++count;
        }
        ++cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t fib_val = fib(n + 4);       /* F_{n+4}  (F0 = 0, F1 = 1) */
    seq_t d_val   = nonlucas_n(n);    /* d(n)    */
    return fib_val + d_val;
}
