/*
 * A124761  Number of falls for compositions in standard order.
 * https://oeis.org/A124761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = number of falls (descents) in the n‑th composition in
 *            standard order
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is defined as 0 */
        return 0;

    idx_t pos = 1;          /* current bit position (1‑based) */
    idx_t prev_pos = 0;     /* last bit position that was set */
    seq_t prev_part = 0;    /* previous part length in the ascending list */
    seq_t count   = 0;      /* number of falls */
    bool first = true;      /* true until the first part is processed */

    idx_t temp = n;
    while (temp != 0) {
        if (temp & 1ULL) {                /* bit is set */
            seq_t part = (seq_t)(pos - prev_pos);
            if (!first) {
                if (part > prev_part)
                    count++;
            }
            prev_part = part;
            prev_pos  = pos;
            first     = false;
        }
        temp >>= 1;
        pos++;
    }
    return count;
}
