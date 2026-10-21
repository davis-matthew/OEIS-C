/*
 * A225822  Lesser of adjacent odd numbers with different parity of binary
 *          weight and both isolated from odd numbers of same parity of binary
 *          weight.
 * https://oeis.org/A225822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = lesser of adjacent odd numbers with different binary
 *            weight parity, both isolated from other odd numbers of the same
 *            parity. This implementation searches successive odd numbers and
 *            tests parity and isolation
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int parity_uint64(uint64_t v) {
    /* parity of the binary weight: 0 for even, 1 for odd */
    return (__builtin_popcountll(v) & 1);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t x = 1;          /* current odd number */

    for (;;) {
        int px = parity_uint64(x);
        int py = parity_uint64(x + 2);  /* parity of the next odd */

        if (px != py) {                 /* different weight parity */
            /* isolation for x: no same-parity odd at distance 2 */
            if (x > 1 && parity_uint64(x - 2) == px)
                goto next;

            /* isolation for y = x+2: no same-parity odd at distance 2 */
            if (parity_uint64(x + 4) == py)
                goto next;

            /* current pair qualifies */
            count++;
            if (count == n)
                return (seq_t)x;        /* lesser of the pair */
        }

next:
        x += 2;                        /* move to the next odd number */
    }
}
