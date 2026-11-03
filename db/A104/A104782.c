/*
 * A104782  Smallest n^e (e>1) containing n in decimal representation.
 * https://oeis.org/A104782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^A045537(n) // smallest e>1 such that decimal
 *            representation of n occurs in n^e
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 14, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 900 (2026-11-01)
 * Helpers:   num_digits0_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only the utilities digit_len.h and seq_to_dec_string.h,
   which are provided by the build.  No other helper functions are required. */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/num_digits0_seq.h"

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}
#include <stdlib.h>
#include <string.h>

/* Return the nth term of the sequence A104782. */
seq_t a(idx_t n)
{
    /* Base case n = 1: 1^e = 1 meets the condition for any e >= 2. */
    if (n == 1)
        return (seq_t)1;

    seq_t base = (seq_t)n;      /* n as seq_t */
    seq_t pow = base * base;    /* start with e = 2 */

    /* Prepare the decimal representation of n (the pattern to search). */
    char pat[64];
    seq_t sn = (seq_t)n;
    seq_to_dec_string(sn, pat, sizeof(pat));

    while (1) {
        /* Convert current power to decimal string to search for pat. */
        idx_t len = num_digits0_seq(pow);
        char *buff = (char *)malloc((size_t)len + 1);
        if (!buff)   /* out‑of‑memory; abort with undefined behaviour */
            abort();

        seq_to_dec_string(pow, buff, (size_t)len + 1);

        /* Check if the pattern occurs as a contiguous substring. */
        if (strstr(buff, pat) != NULL) {
            free(buff);
            return pow;                 /* found the smallest suitable e */
        }
        free(buff);

        /* Increment exponent: pow := pow * n. */
        pow = pow * base;
    }
}
