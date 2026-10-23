/*
 * A028768  Nonsquares mod 55.
 * https://oeis.org/A028768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n‑th positive integer k such that k (mod 55) is not a
 *            quadratic residue
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static unsigned char is_square[55];
    static bool initialized = false;
    if (!initialized) {
        bool s5[5]   = {0};
        bool s11[11] = {0};
        for (unsigned i = 0; i < 5; ++i)
            s5[(i * i) % 5] = true;
        for (unsigned i = 0; i < 11; ++i)
            s11[(i * i) % 11] = true;
        for (unsigned r = 0; r < 55; ++r)
            is_square[r] = s5[r % 5] && s11[r % 11];
        initialized = true;
    }

    idx_t cur   = 1;   /* current integer to test */
    idx_t count = 0;   /* how many nonsquares found so far */

    while (1) {
        ++cur;
        if (!is_square[(size_t)(cur % 55)]) {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
    }
}
