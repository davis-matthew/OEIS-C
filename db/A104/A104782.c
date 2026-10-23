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
 * Helpers:   digit_len, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only the utilities digit_len.h and seq_to_dec_string.h,
   which are provided by the build.  No other helper functions are required. */

#include "../utils/digit_len.h"
#include "../utils/seq_to_dec_string.h"
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
        idx_t len = digit_len(pow);
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
