/*
 * A225580  The sum of all substrings of n (including n).
 * https://oeis.org/A225580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of all contiguous substrings of the decimal
 *            representation of n
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (e.g. a(1980) = 1 + 9 + 8 + 0 + 19 + 98 + 80 + 198 + 980 + 1980 = 3373) */

seq_t a(idx_t n)
{
    /* Handle the (undefined) offset case n==0 gracefully. */
    if (n == 0)
        return 0;

    /* Extract decimal digits of n in reverse order (least significant first). */
    unsigned int d[20];
    idx_t m = 0;                    /* number of digits */
    idx_t tmp = n;
    while (tmp > 0) {
        d[m++] = tmp % 10;
        tmp /= 10;
    }

    seq_t sum = 0;                  /* running total of all substring sums */
    seq_t cur = 0;                  /* sum of substrings ending at current position */

    /* Process digits from most significant to least significant. */
    for (idx_t i = 1; i <= m; ++i) {
        seq_t digit = (seq_t)d[m - i];          /* current digit */
        cur = cur * 10 + digit * (seq_t)i;     /* add new substrings ending here */
        sum += cur;                            /* accumulate */
    }

    return sum;
}
