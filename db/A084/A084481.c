/*
 * A084481  Number of fault-free tilings of a 4 X 2n rectangle with L
 *          tetrominoes.
 * https://oeis.org/A084481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) for n>6 a(1)=2, a(2)=6, a(3)=10,
 *            a(4)=18, a(5)=38, a(6)=84
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* Short‑circuit for the first six terms */
    if (n <= 6) {
        static const seq_t init[6] = {2, 6, 10, 18, 38, 84};
        return init[n-1];
    }

    /* Static memoisation buffer */
    static seq_t        *buf    = NULL;
    static idx_t          buf_sz = 0;
    static idx_t          last   = 0; /* largest index computed (1‑based) */
    static const seq_t    init[6] = {2, 6, 10, 18, 38, 84};

    /* Allocate or grow the buffer if needed */
    if (n > buf_sz) {
        seq_t *new_buf = alloc_seq_array(n);
        if (buf) {
            for (idx_t i = 0; i < buf_sz; ++i) new_buf[i] = buf[i];
            free_seq_array(buf);
        }
        buf    = new_buf;
        buf_sz = n;
    }

    /* Initialise first six terms once */
    if (last == 0) {
        for (idx_t i = 0; i < 6; ++i) buf[i] = init[i];
        last = 6;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + a(i-3) */
        seq_t val = 2 * buf[i-2] + buf[i-4];
        buf[i-1] = val;
    }
    last = n;

    return buf[n-1];
}
