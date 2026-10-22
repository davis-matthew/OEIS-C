/*
 * A003171  Negated discriminants of orders of imaginary quadratic fields with
 *          1 class per genus (a finite sequence).
 * https://oeis.org/A003171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   search over d = -D (d ≡ 0,3 mod 4). d is a term iff every
 *            reduced primitive form (a,b,c) with b^2-4ac = -d is ambiguous (b
 *            = 0, b = a, or a = c), i.e. the class group has exponent <= 2,
 *            i.e. there is one class per genus
 * Range:     u8 n <= 50, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   a3171_g, a3171_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/a3171_g.h"
#include "../utils/a3171_ok.h"

seq_t a(idx_t n) {
    static int terms[256];
    static int cnt = -1;
    if (cnt < 0) {
        long d;
        cnt = 0;
        for (d = 3; d <= 20000 && cnt < 256; d++) {
            if (d % 4 != 0 && d % 4 != 3) continue;
            if (a3171_ok(d)) terms[cnt++] = (int)d;
        }
    }
    if (n < 1 || n > (idx_t)cnt) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
