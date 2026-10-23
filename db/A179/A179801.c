/*
 * A179801  Count of times n appears in number created by stringing together
 *          previous n-1 integers.
 * https://oeis.org/A179801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = count of occurrences of the decimal representation of n
 *            in the string formed by concatenating 1,2,…,n-1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/digits_of.h"

/* Return the number of decimal digits of x (x>0). */

/* Convert x to decimal string in buf (not null terminated). The number
   of written chars is stored in *len. */
static inline void int_to_str(idx_t x, char *buf, int *len)
{
    char tmp[32];
    int l = 0;
    do {
        tmp[l++] = (char)('0' + (x % 10));
        x /= 10;
    } while (x);
    *len = l;
    for (int i = 0; i < l; ++i) buf[i] = tmp[l - 1 - i];
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* no numbers before 1 */
        return 0;

    /* Pattern to search for: decimal representation of n. */
    char pat[32];
    int pat_len;
    int_to_str(n, pat, &pat_len);

    /* Compute total length of the concatenation of 1 … n-1. */
    size_t total_len = 0;
    for (idx_t i = 1; i < n; ++i)
        total_len += (size_t)digits_of(i);

    if (total_len == 0 || pat_len > (int)total_len)
        return 0;

    /* Allocate buffer to hold the concatenated string. */
    char *buf = (char *)malloc(total_len);
    if (!buf)
        return 0;   /* allocation failed */

    /* Fill the buffer. */
    size_t pos = 0;
    for (idx_t i = 1; i < n; ++i)
    {
        int l;
        int_to_str(i, buf + pos, &l);
        pos += (size_t)l;
    }

    /* Count occurrences of pat in buf. */
    size_t count = 0;
    for (size_t i = 0; i + pat_len <= total_len; ++i)
        if (memcmp(buf + i, pat, pat_len) == 0)
            ++count;

    free(buf);
    return (seq_t)count;
}
