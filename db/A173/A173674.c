/*
 * A173674  a(n) = ceiling(A003269(n)/2).
 * https://oeis.org/A173674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4)+a(n-15)-a(n-16)-a(n-19); Initial values:
 *            a(0)=0,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=1,a(6)=2,
 *            a(7)=2,a(8)=3,a(9)=4,a(10)=5,a(11)=7,a(12)=10,a(13)=13,
 *            a(14)=18,a(15)=25,a(16)=35,a(17)=48,a(18)=66,a(19)=91
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static size_t cap = 0;

    /* initial values a(0) through a(19) */
    static const seq_t init[20] = {
        0ULL, 1ULL, 1ULL, 1ULL, 1ULL, 1ULL,
        2ULL, 2ULL, 3ULL, 4ULL, 5ULL,
        7ULL, 10ULL, 13ULL, 18ULL, 25ULL,
        35ULL, 48ULL, 66ULL, 91ULL
    };

    if (n + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = n + 1;
        seq_t *newbuf = seq_realloc(buf, old_cap, new_cap);
        if (!newbuf)
            return 0;   /* memory allocation failure – return 0 as a safe fallback */

        buf = newbuf;
        cap = new_cap;

        for (idx_t i = old_cap; i < new_cap; ++i) {
            if (i <= 19) {
                buf[i] = init[i];
            } else {
                seq_t t1 = buf[i - 1];
                seq_t t2 = buf[i - 4];
                seq_t t3 = buf[i - 15];
                seq_t t4 = buf[i - 16];
                seq_t t5 = buf[i - 19];
                buf[i] = t1 + t2 + t3 - t4 - t5;
            }
        }
    }

    return buf[n];
}
