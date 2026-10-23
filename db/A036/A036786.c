/*
 * A036786  Length of Roman notation for n < length of decimal representation.
 * https://oeis.org/A036786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036786:
   a(n) = the n-th positive integer whose Roman numeral representation
   has a length strictly less than the number of decimal digits of the integer.
   (Offset: 1) */

#include <stddef.h>   /* for size_t */

static const struct {
    seq_t val;
    unsigned int len;   /* length of the Roman token */
} roman_table[] = {
    {900, 2}, {500, 1}, {400, 2}, {100, 1},
    {90, 2},  {50, 1},  {40, 2},  {10, 1},
    {9, 2},   {5, 1},   {4, 2},   {1, 1}
};

/* Return the length of the Roman representation of n. */
static unsigned int roman_len(seq_t n)
{
    unsigned int length = 0;
    seq_t t = n;

    /* Count the 'M' symbols for thousands. */
    while (t >= (seq_t)1000) {
        ++length;
        t -= (seq_t)1000;
    }

    /* Process the remaining < 1000 values. */
    for (size_t i = 0; i < sizeof(roman_table)/sizeof(roman_table[0]); ++i) {
        while (t >= roman_table[i].val) {
            length += roman_table[i].len;
            t -= roman_table[i].val;
        }
    }
    return length;
}

/* Return the number of decimal digits of n. */
static unsigned int decimal_len(seq_t n)
{
    unsigned int d = 0;
    seq_t t = n;
    while (t > (seq_t)0) {
        t /= (seq_t)10;
        ++d;
    }
    return d;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Under the offset this never happens, but guard against it. */
        return (seq_t)0;
    }

    idx_t count = 0;
    seq_t cur = (seq_t)1;

    while (1) {
        unsigned int rlen = roman_len(cur);
        unsigned int dlen = decimal_len(cur);

        if (rlen < dlen) {   /* strictly smaller */
            ++count;
            if (count == n) {
                return cur;
            }
        }
        ++cur;
    }
}
