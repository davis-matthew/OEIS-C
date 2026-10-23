/*
 * A145100  Integers in which no more than half the digits (rounded up) are
 *          the same, for all bases up to ten.
 * https://oeis.org/A145100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n-th positive integer such that in every base b =
 *            2..10 the most frequent digit in the base-b representation does
 *            not exceed ceil(L/2), where L is the number of digits. The
 *            implementation simply enumerates integers and tests this
 *            property for each
 * Range:     u8 n <= 47, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* The sequence is defined for n >= 1 */

    idx_t found = 0;
    seq_t m = 1;

    for (;;) {
        bool ok = true;
        for (int base = 2; base <= 10 && ok; ++base) {
            int cnt[10] = {0};
            int len = 0;
            seq_t x = m;
            while (x > 0) {
                int r = (int)(x % base);
                cnt[r]++;
                len++;
                x /= base;
            }
            int maxf = 0;
            for (int d = 0; d < base; ++d) {
                if (cnt[d] > maxf) maxf = cnt[d];
            }
            if (maxf > (len + 1) / 2) ok = false;
        }
        if (ok) {
            ++found;
            if (found == n) return m;
        }
        ++m;
    }
}
