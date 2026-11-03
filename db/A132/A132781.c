/*
 * A132781  Numbers not containing '1010' in binary representation.
 * https://oeis.org/A132781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the numbers whose binary representation does not contain the
 *            substring "1010". The n-th such number (offset 1) is returned
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9891 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether the binary representation of x contains the substring
   1010.  This is a small routine that examines each 4‑bit window
   of the bit pattern.  It works for any value that fits into
   64 bits, which is sufficient for n up to about 10000. */
static bool has_1010(seq_t x)
{
    /* Work with a 64‑bit view of the value.  If x uses a larger type
       (in the arbitrary‑precision build) the values we examine here
       will always be small and will fit in 64 bits. */
    uint64_t v = (uint64_t)x;
    int len = 0;
    uint64_t t = v;
    while (t) {
        ++len;
        t >>= 1;
    }
    if (len < 4) return false;
    for (int i = len - 4; i >= 0; --i) {
        if (((v >> i) & 0xF) == 0xA) /* 0xA == 1010b */
            return true;
    }
    return false;
}

/* Return the n-th term (1‑based) of sequence A132781. */
seq_t a(idx_t n)
{
    idx_t seen = 0;     /* how many valid numbers have been found */
    seq_t x = 0;        /* candidate integer */

    while (true) {
        if (!has_1010(x)) {
            ++seen;
            if (seen == n)
                return x;
        }
        ++x;
    }
}
