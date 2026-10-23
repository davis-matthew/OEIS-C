/*
 * A137303  Decimal expansion of 9999999/9801.
 * https://oeis.org/A137303
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   init_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A137303 – decimal expansion of 9999999/9801, digits concatenated
    (integer part followed by repeating fractional part).
    a(4)=1, a(5)=0, a(6)=2, a(7)=0, a(8)=3, …                */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/init_sequence.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static unsigned char int_digits[20];
    static size_t int_len = 0;
    static unsigned char cycle[660];
    static size_t cycle_len = 0;

    if (!initialized) {
        init_sequence(int_digits, &int_len, cycle, &cycle_len);
        initialized = true;
    }

    if (n < 4) return (seq_t)0;   /* behaviour below offset is irrelevant */

    idx_t k = n - 4;               /* 0‑based index of the requested digit */

    if ((size_t)k < int_len) {
        return (seq_t)int_digits[(size_t)k];
    }

    /* Fractional part */
    idx_t idx = (k - (idx_t)int_len) % (idx_t)cycle_len;
    return (seq_t)cycle[(size_t)idx];
}
