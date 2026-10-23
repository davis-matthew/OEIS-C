/*
 * A143264  A positive integer n is included if the binary representation of
 *          the number of divisors of n does not occur anywhere in the binary
 *          representation of n.
 * https://oeis.org/A143264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   to_bin_str_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143264: 
   a(n) is the n-th positive integer such that the binary string of the
   number of divisors of the integer does NOT appear anywhere in the
   binary string of the integer itself.  This implementation simply
   scans integers in increasing order, counting the qualifying ones.
   It works for any seq_t and idx_t types provided in the build. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/to_bin_str_uint64.h"

/* Count divisors of a 64‑bit integer. */
static uint64_t divisor_count_uint64(uint64_t k)
{
    uint64_t d = 1;
    for (uint64_t p = 2; p * p <= k; ++p)
    {
        if (k % p == 0)
        {
            uint64_t exp = 0;
            do
            {
                ++exp;
                k /= p;
            } while (k % p == 0);
            d *= (exp + 1);
        }
    }
    if (k > 1) d *= 2;
    return d;
}

/* Return true if pattern exists as a contiguous substring of text. */
static int contains_substring(const char *text, const char *pattern)
{
    const char *p = strstr(text, pattern);
    return p != NULL;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, guard */

    idx_t found = 0;
    uint64_t candidate = 1u;

    /* Use 64‑bit buffers for binary strings. */
    char num_bin[65];
    char div_bin[65];

    while (found < n)
    {
        ++candidate;

        uint64_t divs = divisor_count_uint64(candidate);

        to_bin_str_uint64(candidate, num_bin);
        to_bin_str_uint64(divs, div_bin);

        if (!contains_substring(num_bin, div_bin))
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
    }

    /* Should never reach here. */
    return (seq_t)0;
}
