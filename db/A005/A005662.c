/*
 * A005662  Start with 4; if k appears then so do 2k+2 and 3k+3. (duplicates
 *          omitted.)
 * https://oeis.org/A005662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   generated in increasing order by merging two streams: a(1) = 4;
 *            next term = min(2*a(i)+2, 3*a(j)+3) over pointers i, j
 *            (duplicates omitted)
 * Range:     u8 n <= 27, u16 n <= 2398, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_array, grow_arr, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_array.h"
#include "../utils/grow_arr.h"
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    static seq_t *s = NULL;
    static idx_t len = 0, cap = 0;
    static idx_t i2 = 0, i3 = 0;

    if (s == NULL) {
        cap = 1024;
        s = alloc_array(cap);
        s[0] = 4;
        len = 1;
    }

    bool fw = seq_is_fixed_width();
    seq_t lim2 = 0, lim3 = 0;
    if (fw) {
        seq_t mx = (seq_t)0;
        mx -= 1;
        lim2 = (mx - 2) / 2;
        lim3 = (mx - 3) / 3;
    }

    while (len < n) {
        bool ok2 = !fw || s[i2] <= lim2;
        bool ok3 = !fw || s[i3] <= lim3;
        if (!ok2 && !ok3) break;
        seq_t c2 = 0, c3 = 0;
        if (ok2) c2 = s[i2] * 2 + 2;
        if (ok3) c3 = s[i3] * 3 + 3;
        seq_t next;
        if (ok2 && ok3) next = (c2 < c3) ? c2 : c3;
        else if (ok2) next = c2;
        else next = c3;

        if (len == cap) {
            idx_t ncap = cap * 2;
            s = grow_arr(s, cap, ncap);
            cap = ncap;
        }
        s[len++] = next;
        if (ok2 && c2 == next) i2++;
        if (ok3 && c3 == next) i3++;
    }

    if (n - 1 >= len) return s[len - 1];
    return s[n - 1];
}
