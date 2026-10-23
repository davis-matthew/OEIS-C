/*
 * A138492  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 8.
 * https://oeis.org/A138492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 8.
   For n > 0, a(n) is obtained by reading a(n-1) from right to left,
   saying “<count of digit> <digit>” for each distinct digit encountered.
   This is equivalent to the recurrence a(n) = next_term(a(n-1)). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_to_str.h"

/* Build the next term from the previous one. */
static seq_t next_term(seq_t prev)
{
    /* Buffer big enough for all terms reached in the tests. */
    char buf[1024];
    size_t len = 0;
    seq_to_str(prev, buf, &len);

    /* Count how many times each decimal digit occurs in the whole number. */
    uint64_t cnt[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        ++cnt[buf[i] - '0'];
    }

    /* Scan from right to left, outputting each digit only once. */
    bool seen[10] = {false};
    seq_t result = 0;

    for (int i = (int)len - 1; i >= 0; --i) {
        int d = buf[i] - '0';
        if (!seen[d]) {
            uint64_t c = cnt[d];

            /* Output the decimal representation of the count. */
            char tmp[32];
            int t = 0;
            if (c == 0) {
                tmp[t++] = '0';
            } else {
                uint64_t tmpc = c;
                while (tmpc > 0) {
                    tmp[t++] = '0' + (tmpc % 10);
                    tmpc /= 10;
                }
                /* Reverse to get the correct order. */
                for (int k = t - 1; k >= 0; --k) {
                    result = result * 10 + (seq_t)(tmp[k] - '0');
                }
            }

            /* Append the digit itself. */
            result = result * 10 + (seq_t)d;
            seen[d] = true;
        }
    }
    return result;
}

/* External function used by the build system. */
seq_t a(idx_t n)
{
    seq_t cur = 8;              /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        cur = next_term(cur);
    }
    return cur;
}
