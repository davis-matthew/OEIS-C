/*
 * A096748  Expansion of (1+x)^2/(1-x^2-x^4).
 * https://oeis.org/A096748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-2) + a(n-4)
 * Initial values: a(0)=1, a(1)=2, a(2)=2, a(3)=2
 * This implementation uses a fixed‑size circular buffer of length 5,
 * so it requires only constant memory and works for any supported
 * seq_t type (unsigned integer of any size or arbitrary‑precision
 * integer class in C++).
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n = 0 .. 3 */
    if (n < 4) {
        static const seq_t base[4] = {
            (seq_t)1, /* a(0) */
            (seq_t)2, /* a(1) */
            (seq_t)2, /* a(2) */
            (seq_t)2  /* a(3) */
        };
        return base[(size_t)n];
    }

    /* Circular buffer holding a(i-4) .. a(i) for current i */
    seq_t buf[5];
    buf[0] = (seq_t)1; /* a(0) */
    buf[1] = (seq_t)2; /* a(1) */
    buf[2] = (seq_t)2; /* a(2) */
    buf[3] = (seq_t)2; /* a(3) */
    /* buf[4] will be filled when i == 4 */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        /* Indices for a(i-2) and a(i-4) in the circular buffer */
        idx_t idx_a2 = (i - (idx_t)2) % (idx_t)5;
        idx_t idx_a4 = (i - (idx_t)4) % (idx_t)5;

        seq_t val = buf[idx_a2] + buf[idx_a4];
        buf[i % (idx_t)5] = val;
    }

    return buf[n % (idx_t)5];
}
