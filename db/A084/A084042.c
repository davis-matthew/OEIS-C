/*
 * A084042  Smallest multiple of n in which the digit string of n appears
 *          sandwiched between at least a pair of digits.
 * https://oeis.org/A084042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest multiple of n that contains the decimal string of n
   with at least one digit before and one digit after it. */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_to_str.h"   /* provides seq_to_str */

seq_t a(idx_t n)
{
    /* Convert n to its decimal representation once */
    char buf_n[64];
    size_t len_n;
    seq_to_str((seq_t)n, buf_n, &len_n);

    /* Buffer for multiples; enough for numbers up to many thousands of digits. */
    static char buf_m[4096];
    size_t len_m;
    seq_t sn = (seq_t)n;
    seq_t m;

    for (idx_t k = 1; ; ++k)
    {
        m = sn * (seq_t)k;
        seq_to_str(m, buf_m, &len_m);
        if (len_m < len_n + 2)            /* need ≥ one digit on each side */
            continue;

        /* Search for an occurrence of buf_n that is not at the ends. */
        for (size_t i = 1; i + len_n < len_m; ++i)
        {
            if (memcmp(buf_m + i, buf_n, len_n) == 0)
                return m;                 /* found the smallest suitable multiple */
        }
    }
}
