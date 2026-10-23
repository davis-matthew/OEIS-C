/*
 * A048316  Numbers whose base-5 expansions, read from left to right, have run
 *          lengths that strictly decrease.
 * https://oeis.org/A048316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate natural numbers, keep those whose base‑5 expansions
   read from left to right have strictly decreasing run lengths, and cache
   the results.  The cache grows as needed so that any call a(n) returns
   the n‑th term (a(0)=0). */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint8_t, uint64_t */

/* Return 1 if the base‑5 representation of x has strictly decreasing
   run lengths (left to right).  0 otherwise. */
static int valid(seq_t x)
{
    if (x == 0)
        return 1;               /* 0 is accepted by definition */

    /* Collect digits in base 5 (least significant first). */
    uint8_t digits[2048];
    size_t k = 0;
    seq_t t = x;
    while (t != 0) {
        digits[k++] = (uint8_t)(t % 5);
        t = t / 5;
    }

    int prev_len = -1;                /* length of the run before the current one */
    int cur_digit = digits[k - 1];     /* most significant digit */
    int cur_len = 1;                  /* length of the current run */

    /* Scan from second most significant digit down to the least. */
    for (int i = (int)k - 2; i >= 0; --i) {
        int d = digits[i];
        if (d == cur_digit) {
            /* Same run continues. */
            cur_len++;
        } else {
            /* A new run starts.  The run that just finished must be
               strictly longer than the one after it.  Since we are
               moving towards the least significant digit, the newly
               finished run is the "previous" run.  Thus we compare
               prev_len (earlier run) to cur_len (finished run). */
            if (prev_len != -1 && prev_len <= cur_len)
                return 0;          /* not strictly decreasing */
            prev_len = cur_len;
            cur_digit = d;
            cur_len = 1;
        }
    }

    /* Check the last (least significant) run against the previous one. */
    if (prev_len != -1 && prev_len <= cur_len)
        return 0;

    return 1;                       /* satisfies the run‑length condition */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cap   = 0;      /* capacity of the cache array */
    static size_t len   = 0;      /* number of stored terms */
    static seq_t cur    = 0;      /* last candidate checked */

    /* Generate terms until we reach index n. */
    while (len <= (size_t)n) {
        if (valid(cur)) {
            if (len == cap) {
                cap = cap ? cap * 2 : 128;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = cur;
        }
        cur++;
    }
    return cache[n];
}
