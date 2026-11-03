/*
 * A175928  Number of bases in which the concatenation of the consecutive
 *          increasing integers 1...n is greater than the concatenation of
 *          consecutive decreasing integers n...1 in the same base.
 * https://oeis.org/A175928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of bases b >= 2 such that the base‑b concatenation
 *            of 1,2,…,n is lexicographically greater than the base‑b
 *            concatenation of n,n-1,…,1. The value is obtained by testing
 *            every base b = 2…n and comparing the two concatenations digit by
 *            digit
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 105 terms through n = 106 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static size_t get_digits_uint64(uint64_t x, uint64_t base, int *buf)
{
    /* Produce the digits of x in base 'base' in most‑significant–first order. */
    size_t len = 0;
    do {
        buf[len++] = (int)(x % base);
        x /= base;
    } while (x != 0);

    /* reverse to MSB‑first */
    for (size_t i = 0, j = len - 1; i < j; ++i, --j)
        {
            int t = buf[i];
            buf[i] = buf[j];
            buf[j] = t;
        }
    return len;
}

static int compare_inc_greater(idx_t n, uint64_t base)
{
    /* Compare the concatenation 1…n with n…1 in base 'base'.
       Return 1 if inc > dec, 0 otherwise. */

    /* State for the increasing side */
    uint64_t inc_num = 1;
    int inc_digits[64];
    size_t inc_len = get_digits_uint64(inc_num, base, inc_digits);
    size_t inc_pos = 0;

    /* State for the decreasing side */
    uint64_t dec_num = n;
    int dec_digits[64];
    size_t dec_len = get_digits_uint64(dec_num, base, dec_digits);
    size_t dec_pos = 0;

    while (inc_num <= n && dec_num >= 1)
    {
        int d_inc = inc_digits[inc_pos];
        int d_dec = dec_digits[dec_pos];

        if (d_inc > d_dec)
            return 1;
        if (d_inc < d_dec)
            return 0;

        /* digits equal – advance positions */
        inc_pos++;
        if (inc_pos == inc_len)
        {
            inc_num++;
            if (inc_num > n)
                break;
            inc_len = get_digits_uint64(inc_num, base, inc_digits);
            inc_pos = 0;
        }

        dec_pos++;
        if (dec_pos == dec_len)
        {
            dec_num--;
            if (dec_num < 1)
                break;
            dec_len = get_digits_uint64(dec_num, base, dec_digits);
            dec_pos = 0;
        }
    }
    return 0; /* equal or inc not greater */
}

seq_t a(idx_t n)
{
    /* OEIS offset is 2; values for n < 2 are irrelevant. */
    if (n < 2) return (seq_t)0;

    seq_t count = 0;
    for (idx_t base = 2; base <= n; ++base)
        if (compare_inc_greater(n, (uint64_t)base))
            count++;

    return count;
}
