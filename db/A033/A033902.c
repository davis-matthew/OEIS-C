/*
 * A033902  Sort then Add, a(1)=25.
 * https://oeis.org/A033902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=25; for n>1, a(n)=a(n-1)+sorted_ascending_digits(a(n-1))
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence uses ascending decimal digits, ignoring leading zeros. */
#include <stdbool.h>

/* Return number formed by sorting decimal digits of x in non‑decreasing order,
   dropping leading zeros (e.g. 50 → 5).  Works for any seq_t that supports
   %10, /10, +, *, etc. */
static seq_t sorted_ascending_digits(seq_t x)
{
    unsigned char cnt[10] = {0};
    seq_t tmp = x;

    if (tmp == (seq_t)0) {
        cnt[0] = 1;
    } else {
        while (tmp > (seq_t)0) {
            int d = (int)(tmp % (seq_t)10);
            cnt[d]++;
            tmp /= (seq_t)10;
        }
    }

    seq_t res = (seq_t)0;
    bool started = false;
    for (int d = 0; d <= 9; ++d) {
        while (cnt[d] > 0) {
            if (!started && d == 0) {
                /* skip leading zeros */
                cnt[d]--;
                continue;
            }
            started = true;
            res = res * (seq_t)10 + (seq_t)d;
            cnt[d]--;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;        /* undefined for offset 1 */
    }

    seq_t cur = (seq_t)25;     /* a(1) */
    for (idx_t i = (idx_t)1; i < n; ++i) {
        cur += sorted_ascending_digits(cur);
    }
    return cur;
}
