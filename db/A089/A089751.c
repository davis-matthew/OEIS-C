/*
 * A089751  a(n) = the first prime in the orbit of n under f(n) = n^2 + 1, if
 *          it exists; = 0 otherwise.
 * https://oeis.org/A089751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = first prime in the orbit of n under f(n) = n^2 + 1, or 0
 *            if none found
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    if (isPrimeGeneric(x))
        return x;
    for (idx_t step = 1; step < 100000; ++step) {
        x = x * x + 1;
        if (isPrimeGeneric(x))
            return x;
    }
    return 0;
}
