/*
 * A136820  Numbers k such that k and k^2 use only the digits 0, 1, 2, 4 and
 *          8.
 * https://oeis.org/A136820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 177, u64 n <= 1047,
 *            u128 n <= 1047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: generate all positive integers composed of the digits
   0,1,2,4,8 (excluding the leading zero).  For each candidate k
   check if k² uses only those digits as well.  Count such numbers
   until reaching the desired index n.  Term a(1) is defined
   separately as 0.  The routine works for any seq_t size
   (uint8_t,…, uint64_t, unsigned __int128, arbitrary precision).  */

#include <stdbool.h>

static const int allowed_digits[5] = {0, 1, 2, 4, 8};

static bool digits_ok(seq_t x)
{
    /* True if every decimal digit of x is in {0,1,2,4,8}. */
    if (x == 0) return true;
    while (x > 0) {
        int d = (int)(x % 10);
        if (!(d == 0 || d == 1 || d == 2 || d == 4 || d == 8))
            return false;
        x /= 10;
    }
    return true;
}

/* Recursive generator that enumerates all numbers of a fixed length
   using allowed_digits (no leading zero).  On success (cnt reaches
   target) *res is set and the function returns true to stop further
   recursion. */
static bool gen_len(idx_t len, idx_t pos, seq_t cur,
                    idx_t *cnt, idx_t target, seq_t *res)
{
    if (pos == len) {
        seq_t sq = cur * cur;
        if (digits_ok(sq)) {
            (*cnt)++;
            if (*cnt == target) {
                *res = cur;
                return true;
            }
        }
        return false;
    }

    for (int i = 0; i < 5; ++i) {
        int d = allowed_digits[i];
        if (pos == 0 && d == 0) continue;          /* no leading zero */
        seq_t next = cur * 10;
        next += d;                                  /* next = cur*10 + d */
        if (gen_len(len, pos + 1, next, cnt, target, res))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* a(1) = 0 */

    idx_t count = 1;                      /* zero already counted */
    seq_t result = 0;

    /* Try increasing length until the n‑th term is found. */
    for (idx_t len = 1; ; ++len) {
        if (gen_len(len, 0, 0, &count, n, &result))
            return result;
        /* For very large n, lengths may grow rapidly; the loop will
           eventually terminate when the count reaches n.              */
    }
}
