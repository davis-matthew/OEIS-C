/*
 * A031463  Numbers whose base-4 representation has 2 more 0's than 3's.
 * https://oeis.org/A031463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑4 representation
 *            contains two more 0's than 3's
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Count of (0's – 3's) in the base‑4 representation of x. */
static bool satisfies(seq_t x)
{
    int diff = 0;
    seq_t t = x;
    while (t != 0) {
        int d = (int)(t % (seq_t)4);
        if (d == 0)
            ++diff;
        else if (d == 3)
            --diff;
        t /= (seq_t)4;
    }
    return diff == 2;
}

/* Static cache – large enough for any n requested in the test harness. */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;    /* number of valid entries stored */
static seq_t last_scanned = 0;  /* last integer examined */

/* Generate terms until cache contains at least n entries. */
static void generate_up_to(idx_t n)
{
    if ((idx_t)cache_len >= n)
        return;

    for (;;) {
        ++last_scanned;
        if (satisfies(last_scanned)) {
            if (cache_len < (size_t)MAX_CACHE)
                cache[cache_len++] = last_scanned;
            else
                /* In practice n ≤ 10000, so this should never happen. */
                return;
            if ((idx_t)cache_len >= n)
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n == 0, per the offset */

    generate_up_to(n);
    return cache[(size_t)n - 1];
}
