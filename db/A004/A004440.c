/*
 * A004440  Numbers that are not the sum of 3 distinct nonzero squares.
 * https://oeis.org/A004440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every a^2+b^2+c^2 with 0<a<b<c below a limit, then
 *            take the n-th unmarked positive integer; the limit doubles until
 *            n unmarked numbers are found
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


static bool *s_mark = NULL;
static idx_t s_limit = 0;

static void build(idx_t limit) {
    s_mark = (bool *)realloc(s_mark, (size_t)limit * sizeof(bool));
    memset(s_mark, 0, (size_t)limit * sizeof(bool));
    for (idx_t x = 1; 3 * x * x < limit; x++) {
        for (idx_t y = x + 1; x * x + 2 * y * y < limit; y++) {
            for (idx_t z = y + 1; ; z++) {
                idx_t s = x * x + y * y + z * z;
                if (s >= limit) break;
                s_mark[s] = true;
            }
        }
    }
    s_limit = limit;
}

seq_t a(idx_t n) {
    idx_t limit = s_limit;
    if (limit < 6 * n + 200) limit = 6 * n + 200;
    for (;;) {
        if (limit > s_limit) build(limit);
        idx_t count = 0;
        for (idx_t i = 1; i < s_limit; i++) {
            if (!s_mark[i]) {
                count++;
                if (count == n) return (seq_t)i;
            }
        }
        limit = s_limit * 2;
    }
}
