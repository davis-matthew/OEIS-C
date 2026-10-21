/*
 * A088344  Powers of 3 grouped in pairs of 2 digits.
 * https://oeis.org/A088344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th two‑digit block obtained by concatenating the
 *            decimal representations of successive powers of 3 (3¹, 3², …)
 *            and splitting that infinite string into pairs of digits from
 *            left to right
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n < 1 is irrelevant */

    /* Buffers sufficient for powers up to 3⁴⁰⁰ (≈ 190 digits) and for the
       temporary stream of digits. */
    char power[512];
    size_t power_len = 1;
    power[0] = '3';

    char buf[512];
    size_t buf_len = 0;

    idx_t term_idx = 1;

    while (1)
    {
        /* Append current power to the digit stream. */
        for (size_t i = 0; i < power_len; ++i)
            buf[buf_len++] = power[i];

        /* Emit two‑digit terms from the front of the stream. */
        while (buf_len >= 2)
        {
            int val = (buf[0] - '0') * 10 + (buf[1] - '0');
            if (term_idx == n) return (seq_t)val;
            ++term_idx;

            /* Shift the remaining digits left by two positions. */
            memmove(buf, buf + 2, buf_len - 2);
            buf_len -= 2;
        }

        /* Prepare the next power: multiply the current power by 3. */
        int carry = 0;
        for (int i = (int)power_len - 1; i >= 0; --i)
        {
            int prod = (power[i] - '0') * 3 + carry;
            power[i] = '0' + (prod % 10);
            carry    = prod / 10;
        }
        if (carry)
        {
            /* Shift existing digits right to make room for the carry. */
            memmove(power + 1, power, power_len);
            power[0] = '0' + carry;
            ++power_len;
        }
    }
}
