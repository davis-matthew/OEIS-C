/*
 * A002710  Triangulations of the disk G_{n,1}.
 * https://oeis.org/A002710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (T(n) + R2(n) + 2*R4(n))/4 (Burnside over the 4 rotations
 *            of the square), where T(n) = 40*(4n+3)!/(n!*(3n+5)!) = rooted
 *            count (Brown), R2(2k) = 4*C(4k+1,k)/(3k+2), R2(2k+1) =
 *            4*C(4k+3,k)/(3k+4) (half-turn symmetric), R4(4j+1) =
 *            2*C(4j+1,j)/(3j+2), R4(n) = 0 otherwise (quarter-turn symmetric)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   a2710_bterm, a2710_div, a2710_fterm, a2710_isbig, a2710_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2710_bterm.h"
#include "../utils/a2710_div.h"
#include "../utils/a2710_fterm.h"
#include "../utils/a2710_isbig.h"
#include "../utils/a2710_mul.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    if (a2710_isbig()) {
        seq_t t = a2710_bterm(4 * n + 3, n, 40, 3 * n + 5, 3 * n + 4);
        seq_t r2, r4 = 0;
        if (n % 2 == 0) r2 = a2710_bterm(2 * n + 1, k, 4, 3 * k + 2, 1);
        else r2 = a2710_bterm(2 * n + 1, k, 4, 3 * k + 4, 1);
        if (n % 4 == 1) {
            idx_t j = n / 4;
            r4 = a2710_bterm(4 * j + 1, j, 4, 3 * j + 2, 1);
        }
        return (t + r2 + r4) / 4;
    } else {
        uint64_t acc[3];
        seq_t r;
        int i;
        acc[0] = 0; acc[1] = 0; acc[2] = 0;
        a2710_fterm(acc, 4 * n + 3, n, 40, 3 * n + 5, 3 * n + 4);
        if (n % 2 == 0) a2710_fterm(acc, 2 * n + 1, k, 4, 3 * k + 2, 1);
        else a2710_fterm(acc, 2 * n + 1, k, 4, 3 * k + 4, 1);
        if (n % 4 == 1) {
            idx_t j = n / 4;
            a2710_fterm(acc, 4 * j + 1, j, 4, 3 * j + 2, 1);
        }
        a2710_div(acc, 4);
        r = (seq_t)acc[1];
        for (i = 0; i < 8; i++) r = (seq_t)(r << 8);
        r |= (seq_t)acc[0];
        return r;
    }
}
