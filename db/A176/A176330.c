/*
 * A176330  The largest Armstrong number
 *          115132219018763992565095597973971522401 written in base 2.
 * https://oeis.org/A176330
 *
 * Offset:    127    (a(127) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = (n >= 127) ? bit(n-127 of
 *            115132219018763992565095597973971522401) : 0
 * Range:     u8 n <= 231, u16 n <= 231, u32 n <= 231, u64 n <= 231,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence returns the binary expansion of the largest known Armstrong
   number, with index 127 corresponding to the most significant bit. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is undefined;
       we simply return 0. */
    if (n < 127)
        return (seq_t)0;

    /* Static storage for the binary digits, filled once. */
    static unsigned char bits[200];
    static size_t bits_len = 0;
    static int initialized = 0;

    if (!initialized)
    {
        /* Decimal representation of the Armstrong number. */
        const char *dec =
            "115132219018763992565095597973971522401";

        /* Big integer stored in base 1e9, little‑endian. 15 words are
           sufficient for a 39‑digit decimal number. */
        uint32_t seg[15] = {0};
        size_t seg_len = 0;

        /* Convert the decimal string to the big integer. */
        for (size_t i = 0; dec[i] != '\0'; ++i)
        {
            uint32_t d = (uint32_t)(dec[i] - '0');
            uint64_t carry = d;
            for (size_t j = 0; j < seg_len; ++j)
            {
                uint64_t tmp = (uint64_t)seg[j] * 10 + carry;
                seg[j] = (uint32_t)(tmp % 1000000000u);
                carry  = tmp / 1000000000u;
            }
            while (carry > 0)
            {
                seg[seg_len++] = (uint32_t)(carry % 1000000000u);
                carry /= 1000000000u;
            }
        }
        if (seg_len == 0)
            seg_len = 1;

        /* Extract bits by repeatedly dividing by 2.  Bits are collected
           least‑significant first. */
        unsigned char tmp_bits[200];
        size_t tmp_len = 0;
        while (seg_len > 0)
        {
            uint64_t carry = 0;
            for (int i = (int)seg_len - 1; i >= 0; --i)
            {
                uint64_t cur = (uint64_t)seg[i] + carry * 1000000000u;
                seg[i] = (uint32_t)(cur / 2);
                carry = cur % 2;
            }
            while (seg_len > 0 && seg[seg_len - 1] == 0)
                --seg_len;
            tmp_bits[tmp_len++] = (unsigned char)carry;  /* LSB first */
        }

        /* Reverse to obtain most‑significant‑bit first order. */
        bits_len = tmp_len;
        for (size_t i = 0; i < tmp_len; ++i)
            bits[i] = tmp_bits[tmp_len - 1 - i];

        initialized = 1;
    }

    size_t idx = (size_t)(n - 127);
    if (idx >= bits_len)
        return (seq_t)0;
    return (seq_t)bits[idx];
}
