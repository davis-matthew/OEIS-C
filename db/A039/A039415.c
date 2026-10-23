/*
 * A039415  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 8's.
 * https://oeis.org/A039415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039415: numbers whose base‑10 representation has the same nonzero
 * number of 2's and 8's.  The function searches successive natural numbers
 * and keeps those where count(2) == count(8) > 0.  Results are memoised in a
 * static array so that calls with arbitrary n are efficient. */
#include <stdlib.h>
#include <stdbool.h>

/* Count the decimal digits of x.  The array cnt must be of length 10. */
static void count_digits(seq_t x, unsigned char cnt[10])
{
    unsigned char local_cnt[10] = {0};
    /* Handle zero explicitly – it never satisfies the condition. */
    if (x == (seq_t)0) {
        for (int i = 0; i < 10; ++i) cnt[i] = local_cnt[i];
        return;
    }
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        ++local_cnt[d];
        x /= (seq_t)10;
    }
    for (int i = 0; i < 10; ++i) cnt[i] = local_cnt[i];
}

seq_t a(idx_t n)
{
    /* Safety for n == 0, even though the offset is 1. */
    if (n == 0) return (seq_t)0;

    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t count  = 0;       /* number of terms stored */
    static size_t cap    = 0;       /* allocated size */
    static seq_t next    = 1;       /* next integer to test */

    /* Initialise storage on the first call. */
    if (count == 0) {
        cap    = 16;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we reach the desired index. */
    while (count < n) {
        unsigned char digits[10] = {0};
        count_digits(next, digits);
        if (digits[2] == digits[8] && digits[2] > 0) {
            /* Ensure capacity. */
            if (count >= cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = next;
        }
        ++next;
    }

    return cache[n - 1];
}
