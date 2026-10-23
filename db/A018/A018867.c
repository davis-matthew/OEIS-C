/*
 * A018867  Smallest power of 8 that begins with n.
 * https://oeis.org/A018867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest power of 8 that begins with n (decimal)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6, u64 n <= 6, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "../utils/seq_pow.h"      /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/seq_to_str.h"   /* void seq_to_str(seq_t v, char *buf, size_t *len); */

seq_t a(idx_t n)
{
    /* Convert the index to a decimal string and remember its length */
    char n_str[32];
    int d = snprintf(n_str, sizeof n_str, "%" PRIu64, (uint64_t)n);
    if (d <= 0) return (seq_t)0;  /* defensive; should never happen */

    /* Buffer for the decimal representation of 8^e.  8^10000 has ≈9030 digits. */
    char buf[12000];
    size_t len;

    for (idx_t e = 0; ; ++e)
    {
        /* Compute 8^e as a seq_t. */
        seq_t val = seq_pow((seq_t)8, e);

        /* Convert the value to a decimal string. */
        seq_to_str(val, buf, &len);

        /* Skip if the power has fewer digits than n (then it can't match). */
        if (len < (size_t)d) continue;

        /* Compare the leading 'd' digits. */
        if (strncmp(buf, n_str, d) == 0)
            return val;
    }
}
