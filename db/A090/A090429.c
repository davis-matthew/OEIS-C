/*
 * A090429  3-digit numbers that do not resolve to 495 under the Kaprekar
 *          routine.
 * https://oeis.org/A090429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   numbers found by exhaustive search among 3‑digit integers whose
 *            maximum digit minus minimum digit is ≤ 1. These are exactly the
 *            3‑digit numbers that do NOT resolve to 495 under the Kaprekar
 *            routine (with no leading zeros)
 * Range:     u8 n <= 14, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static seq_t values[900];
static size_t count = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        for (int x = 100; x <= 999; ++x) {
            int a = x / 100;
            int b = (x / 10) % 10;
            int c = x % 10;
            int min = a;
            if (b < min) min = b;
            if (c < min) min = c;
            int max = a;
            if (b > max) max = b;
            if (c > max) max = c;
            if (max - min <= 1) {
                values[count++] = (seq_t)x;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > count)
        return 0;          /* undefined for n beyond the range */

    return values[n - 1];
}
