/*
 * A095604  a(n) = 107 written in base n.
 * https://oeis.org/A095604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of decimal 107 in base n, interpreted as a
 *            decimal integer (for n=1: a(n) is the 107‑digit number
 *            consisting of all 1’s)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the unary case (base 1) specially. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 107; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* 107 is a constant; treat it as a seq_t during the conversion. */
    seq_t val = (seq_t)107;
    /* We can store each digit in a small array because 107 is < 2^7 in any base >= 2. */
    idx_t digits[10];
    idx_t pos = 0;

    /* Repeatedly divide by the base and collect remainders (digits). */
    while (val > 0) {
        seq_t r = val % n;   /* r < n, but ≤107 so fits in idx_t */
        digits[pos++] = (idx_t)r;
        val /= n;
    }

    /* Build the decimal integer from the most significant digit to the least. */
    seq_t res = 0;
    for (idx_t i = pos; i > 0; --i)
        res = res * 10 + digits[i - 1];

    return res;
}
