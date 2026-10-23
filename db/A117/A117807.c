/*
 * A117807  Start with 1015 and repeatedly reverse the digits and add 4 to get
 *          the next term.
 * https://oeis.org/A117807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1015, a(n)=reverse_digits(a(n-1))+4 for n>1
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    static seq_t period[90];
    static bool init = false;
    if (!init) {
        seq_t cur = (seq_t)1015;
        for (size_t i = 0; i < 90; ++i) {
            period[i] = cur;
            cur = reverse_digits(cur) + (seq_t)4;
        }
        init = true;
    }
    size_t idx = (size_t)((n - 1) % 90);
    return period[idx];
}
