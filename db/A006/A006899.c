/*
 * A006899  Numbers of the form 2^i or 3^j.
 * https://oeis.org/A006899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   merge of the two ordered lists 2^i (i>=0) and 3^j (j>=0), with 1
 *            = 2^0 = 3^0 counted once; a(1) = 1
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 104,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t p2 = 1, p3 = 1;
    bool o2 = false, o3 = false; /* side has exceeded the range of seq_t */

    for (idx_t k = 1; k < n; k++) {
        bool adv2, adv3;
        if (o2) {
            adv2 = false; adv3 = true;
        } else if (o3) {
            adv2 = true; adv3 = false;
        } else if (p2 < p3) {
            adv2 = true; adv3 = false;
        } else if (p3 < p2) {
            adv2 = false; adv3 = true;
        } else {
            adv2 = true; adv3 = true;
        }
        if (adv2) {
            seq_t q = p2 * 2;
            if (q / 2 != p2) o2 = true; else p2 = q;
        }
        if (adv3) {
            seq_t q = p3 * 3;
            if (q / 3 != p3) o3 = true; else p3 = q;
        }
    }

    if (o2) return p3;
    if (o3) return p2;
    return (p2 < p3) ? p2 : p3;
}
