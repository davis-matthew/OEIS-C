/*
 * A178983  The smallest cube containing n as a substring.
 * https://oeis.org/A178983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = smallest cube whose decimal representation contains n as
 *            a substring
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Convert a seq_t value to its decimal string representation.
   The result is written into buf, and *len is set to the number
   of decimal digits.  The caller must provide a buffer large
   enough to hold the string plus a terminating NUL. */
static void seq_to_str(seq_t value, char *buf, size_t *len)
{
    if (value == 0)
    {
        buf[0] = '0';
        *len = 1;
        return;
    }

    seq_t v = value;
    char tmp[256];
    size_t pos = 0;

    while (v > 0)
    {
        unsigned int d = (unsigned int)(v % 10);
        tmp[pos++] = (char)('0' + d);
        v = v / 10;
    }

    /* reverse into output buffer */
    for (size_t i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Return the smallest cube that contains n as a decimal substring.
   For n = 0 the sequence definition gives a(0) = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* string representation of the index n */
    char n_str[256];
    size_t n_len;
    seq_to_str((seq_t)n, n_str, &n_len);

    /* iterate over cubes until one is found that contains n_str */
    for (idx_t k = 1; ; ++k)
    {
        seq_t cube = (seq_t)k * k * k;

        char cube_str[256];
        size_t cube_len;
        seq_to_str(cube, cube_str, &cube_len);

        if (cube_len < n_len)
            continue;                /* cube too short to contain the substring */

        for (size_t pos = 0; pos + n_len <= cube_len; ++pos)
        {
            bool match = true;
            for (size_t t = 0; t < n_len; ++t)
            {
                if (cube_str[pos + t] != n_str[t])
                {
                    match = false;
                    break;
                }
            }
            if (match)
                return cube;         /* minimal such cube found */
        }
    }
}
