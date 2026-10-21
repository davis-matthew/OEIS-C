/*
 * A095500  a(n) = 54 written in base n.
 * https://oeis.org/A095500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of decimal number 54 in base n (digits are
 *            written as a decimal integer)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base 1 (unary) separately: 54 ones. */
    if (n == 1) {
        seq_t res = 0;
        /* 54 repetitions of digit 1 */
        for (idx_t i = 0; i < 54; ++i)
            res = res * 10 + 1;
        return res;
    }

    seq_t base = n;
    seq_t val  = 54;          /* decimal value to convert */

    /* The representation of 54 in any base n ≤ 54 uses at most 3 digits,
       but we allocate a small fixed array to be safe. */
    seq_t digits[10];
    int cnt = 0;

    /* Extract digits (least‑significant first). */
    while (val > 0) {
        seq_t d = val % base;
        val = val / base;
        digits[cnt++] = d;
    }

    /* Build the decimal integer from the digits (most‑significant first). */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
