/*
 * A061581  a(1) = 1, a(n) = number obtained by replacing each digit of a(n-1)
 *          with its double.
 * https://oeis.org/A061581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n > 1, a(n) is the decimal number obtained by
 *            replacing each digit d of a(n-1) with its double (2*d)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Handle the offset; n >= 1 is guaranteed by the caller. */
    if (n == 0)
        return 0;  /* undefined, but defined for safety */

    /* Current term stored as an array of decimal digits (most‑significant first). */
    size_t len = 1;                     /* number of digits in current term */
    uint8_t *digits = (uint8_t *)malloc(sizeof(uint8_t));
    digits[0] = 1;                      /* a(1) = 1 */

    /* Temporary buffer used for the next term; grows as needed. */
    uint8_t *buf = NULL;
    size_t buf_cap = 0;

    /* Generate terms up to n. */
    for (idx_t step = 1; step < n; ++step)
    {
        /* The next term can be at most twice as long as the current one. */
        size_t needed = len * 2;
        if (buf_cap < needed)
        {
            buf_cap = needed;
            buf = (uint8_t *)realloc(buf, buf_cap * sizeof(uint8_t));
        }

        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint8_t d   = digits[i];
            uint8_t dd  = d << 1; /* 2 * d */
            if (dd < 10)
                buf[new_len++] = dd;
            else
            {
                buf[new_len++] = dd / 10;
                buf[new_len++] = dd % 10;
            }
        }

        /* Swap buffers for the next iteration. */
        uint8_t *tmp = digits;
        digits      = buf;
        buf         = tmp;
        len         = new_len;
    }

    /* Convert the final digit array into a seq_t value. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
        result = result * 10 + digits[i];

    /* Free allocated memory. */
    free(digits);
    free(buf);

    return result;
}
