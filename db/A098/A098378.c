/*
 * A098378  Number of characters needed to write number n in the traditional
 *          Ethiopic (Geez) number system.
 * https://oeis.org/A098378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of Ethiopic characters needed to write number n in
 *            the traditional Ethiopic (Geez) number system
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   ethiopic_multiplier_count, ethiopic_unit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ethiopic_multiplier_count.h"
#include "../utils/ethiopic_unit_count.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;     /* offset starts at 1, but harmless */
    seq_t cnt = 0;
    idx_t k = 0;
    idx_t rem = n;
    while (rem > 0) {
        idx_t d = rem % 100;          /* current 100‑block */
        if (k == 0) {                 /* units block */
            cnt += ethiopic_unit_count(d);
        } else {                      /* higher blocks */
            if (d != 0) {
                if (d == 1) {
                    /* block value 1 -> only the multiplier character */
                    cnt += ethiopic_multiplier_count(k);
                } else {
                    /* block value >1 -> units + multiplier */
                    cnt += ethiopic_unit_count(d);
                    cnt += ethiopic_multiplier_count(k);
                }
            }
        }
        rem /= 100;
        k++;
    }
    return cnt;
}
