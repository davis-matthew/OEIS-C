/*
 * A044953  Numbers with no two equally numerous base 4 digits.
 * https://oeis.org/A044953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with no two equally numerous base 4 digits. */
#include <stdbool.h>

static inline bool valid(seq_t x)
{
    int cnt[4] = {0};
    seq_t four = 4;
    if (x == 0) return false;
    while (x > 0)
    {
        int d = (int)(x % four);
        cnt[d]++;
        x = x / four;
    }
    for (int i = 0; i < 4; ++i)
    {
        if (cnt[i] == 0) continue;
        for (int j = i + 1; j < 4; ++j)
        {
            if (cnt[j] == 0) continue;
            if (cnt[i] == cnt[j]) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t curr = 1;
    idx_t found = 0;
    for (;;)
    {
        if (valid((seq_t)curr))
        {
            ++found;
            if (found == n)
                return (seq_t)curr;
        }
        ++curr;
    }
}
