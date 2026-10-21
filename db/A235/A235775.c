/*
 * A235775  a(n) = A047842(A047842(n)), say what you see, once repeated.
 * https://oeis.org/A235775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A047842(A047842(n))
 * Range:     u8 none, u16 n <= 19, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047842: for each decimal digit d, count its occurrences in the input,
   then write the counts and the digits increasingly. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/digit_counts.h"
#include "../utils/seq_from_str.h"

/* Helper that implements A047842 on a seq_t value. */
static seq_t A047842(seq_t v) {
    unsigned char cnt[10];
    if (v == (seq_t)0) {
        /* Treat zero as having one zero digit. */
        memset(cnt, 0, sizeof cnt);
        cnt[0] = 1;
    } else {
        digit_counts(v, cnt);
    }

    /* Build the decimal string representation of the output. */
    char buf[128];
    size_t pos = 0;

    for (unsigned d = 0; d < 10; ++d) {
        unsigned char c = cnt[d];
        if (c == 0)
            continue;

        /* Convert count c to decimal digits. */
        char tmp[4];   /* max 3 decimal digits for c<=20 */
        size_t t = 0;
        unsigned char x = c;
        while (x > 0) {
            tmp[t++] = '0' + (x % 10);
            x /= 10;
        }
        /* Append count in correct order. */
        for (size_t i = 0; i < t; ++i)
            buf[pos++] = tmp[t - 1 - i];

        /* Append the digit itself. */
        buf[pos++] = '0' + d;
    }

    buf[pos] = '\0';
    return seq_from_str(buf);
}

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t y = A047842(x);
    seq_t z = A047842(y);
    return z;
}
