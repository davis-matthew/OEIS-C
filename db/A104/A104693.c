/*
 * A104693  Numbers n such that in n^3, number of odd digits is larger than
 *          number of even digits.
 * https://oeis.org/A104693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   A104693 – numbers n such that in n³ the decimal representation
 *            contains more odd digits than even digits
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 837, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 993 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

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

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1 – behaviour below the offset is irrelevant */

    idx_t found = 0;      /* how many qualifying numbers we have seen */
    seq_t cur = 1;        /* candidate integer to test */

    for (;; ++cur)
    {
        seq_t cube = cur * cur * cur;

        /* convert cube to a decimal string */
        char buf[256];
        seq_to_dec_string(cube, buf, sizeof buf);

        /* count odd vs. even digits */
        int diff = 0;      /* odd count – even count */
        for (char *p = buf; *p; ++p)
        {
            int d = *p - '0';
            diff += (d & 1) ? 1 : -1;
        }

        if (diff > 0)      /* more odd digits than even digits */
        {
            ++found;
            if (found == n)
                return cur;
        }
    }
}
