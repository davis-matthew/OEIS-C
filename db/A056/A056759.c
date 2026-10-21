/*
 * A056759  The 17 prime powers k = p^w such that d(p^w)^3 > p^w where d =
 *          A000005().
 * https://oeis.org/A056759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A056759
   Prime powers k = p^w such that d(k)^3 > k, where d(k) is
   the divisor function (for prime powers d(p^w) = w + 1).
   a(1) = 2, a(2) = 3, a(3) = 4, a(4) = 5, a(5) = 7,
   a(6) = 8, a(7) = 9, a(8) = 16, a(9) = 25, a(10) = 27,
   a(11) = 32, a(12) = 64, a(13) = 81, a(14) = 128,
   a(15) = 256, a(16) = 512, a(17) = 1024.
   The sequence contains exactly 17 terms. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 17 terms of the sequence */
    static const seq_t values[17] = {
        2ull, 3ull, 4ull, 5ull, 7ull,
        8ull, 9ull, 16ull, 25ull, 27ull,
        32ull, 64ull, 81ull, 128ull, 256ull,
        512ull, 1024ull
    };

    if (n == 0 || n > (idx_t)sizeof(values)/sizeof(values[0]))
        return 0;   /* undefined for n <= 0 or beyond the known terms */

    return values[n - 1];
}
