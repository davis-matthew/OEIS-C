/*
 * A045734  Smallest Fibonacci number beginning "n^2".
 * https://oeis.org/A045734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest Fibonacci number whose decimal representation begins with n^2 */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_to_str.h"

/* Return true if the decimal representation of f starts with prefix_buf[0..len-1] */
static bool has_prefix(seq_t f, const char *prefix_buf, size_t prefix_len)
{
    /* Buffer must be large enough for the maximum number of digits encountered
       when n ≤ 10000. Fib(10000) has about 2090 decimal digits, so 3000 is safe. */
    static char fbuf[3000];
    size_t flen = 0;
    seq_to_str(f, fbuf, &flen);
    if (flen < prefix_len)
        return false;
    return memcmp(fbuf, prefix_buf, prefix_len) == 0;
}

seq_t a(idx_t n)
{
    /* Compute the required prefix string for n^2 */
    seq_t prefix_val = (seq_t)n * (seq_t)n;
    char prefix_buf[32];
    size_t prefix_len = 0;
    seq_to_str(prefix_val, prefix_buf, &prefix_len);

    /* Generate Fibonacci numbers until one starts with the prefix. */
    seq_t prev = (seq_t)1;   /* F1 */
    seq_t curr = (seq_t)1;   /* F2 */

    if (has_prefix(prev, prefix_buf, prefix_len))
        return prev;
    if (has_prefix(curr, prefix_buf, prefix_len))
        return curr;

    while (true) {
        seq_t next = prev + curr;
        if (has_prefix(next, prefix_buf, prefix_len))
            return next;
        prev = curr;
        curr = next;
    }
}
