/*
 * A039156  Numbers whose base-11 representation has the same number of 0's
 *          and 1's.
 * https://oeis.org/A039156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑11 representation has the same number of 0's
 *            and 1's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t cache[50000];
static size_t cache_size = 0;
static seq_t next_candidate = 1;

seq_t a(idx_t n)
{
    /* ensure the cache contains at least n terms (1‑based index) */
    while (cache_size < (size_t)n) {
        /* count zero and one digits in base‑11 representation of next_candidate */
        seq_t v = next_candidate;
        int zeros = 0, ones = 0;
        seq_t tmp = v;

        /* treat zero explicitly – though it never occurs in this sequence */
        if (tmp == 0) {
            zeros = 1;
        } else {
            while (tmp > 0) {
                int digit = (int)(tmp % 11);   /* 0 … 10 */
                if (digit == 0) ++zeros;
                else if (digit == 1) ++ones;
                tmp /= 11;
            }
        }

        if (zeros == ones) {
            cache[cache_size++] = v;
        }
        ++next_candidate;
    }
    return cache[n - 1];  /* a(1) is stored at cache[0] */
}
