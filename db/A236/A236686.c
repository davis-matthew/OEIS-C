/*
 * A236686  Possible values for positive integers a,b,c,d such that 1/a + 1/b
 *          + 1/c + 1/d = 1.
 * https://oeis.org/A236686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for all solutions of 1/a + 1/b + 1/c + 1/d = 1,
   collect all distinct numbers that appear as any of a,b,c,d,
   sort them, and return the nth (1‑based) element. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static int cmp_seq(const void *p, const void *q)
{
    seq_t va = *(const seq_t *)p;
    seq_t vb = *(const seq_t *)q;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t values[256];
    static size_t count = 0;
    static bool ready = false;

    if (!ready) {
        for (idx_t a = 2; a <= 200; ++a) {
            for (idx_t b = a; b <= 200; ++b) {
                for (idx_t c = b; c <= 200; ++c) {
                    unsigned __int128 num = (unsigned __int128)a * b * c;
                    unsigned __int128 denom = num
                        - (unsigned __int128)a * b
                        - (unsigned __int128)a * c
                        - (unsigned __int128)b * c;
                    if (denom == 0) continue;
                    if (num % denom != 0) continue;
                    unsigned __int128 d128 = num / denom;
                    idx_t d = (idx_t)d128;
                    if (d < c) continue;

                    idx_t vals[4] = { a, b, c, d };
                    for (int i = 0; i < 4; ++i) {
                        seq_t v = (seq_t)vals[i];
                        bool found = false;
                        for (size_t j = 0; j < count; ++j) {
                            if (values[j] == v) { found = true; break; }
                        }
                        if (!found && count < 256) {
                            values[count++] = v;
                        }
                    }
                }
            }
        }
        qsort(values, count, sizeof(seq_t), cmp_seq);
        ready = true;
    }

    if (n < 1 || n > count) return (seq_t)0;
    return values[n - 1];
}
