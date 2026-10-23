/*
 * A036788  Length of Roman notation for n <= length of decimal
 *          representation.
 * https://oeis.org/A036788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) is the n‑th positive integer m such that roman_len(m) <=
 *            digit_len(m)
 * Range:     u8 n <= 34, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* count decimal digits of an unsigned integer */
static inline idx_t digit_len(idx_t x)
{
    idx_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* length of the naive Roman representation of an unsigned integer */
static inline idx_t roman_len(idx_t x)
{
    /*  ones:  I  II  III  IV  V   VI  VII  VIII  IX */
    static const idx_t len_units[10] = {0,1,2,3,2,1,2,3,4,2};
    /*  tens:  X  XX  XXX  XL  L   LX  LXX  LXXX  XC */
    static const idx_t len_tens[10]  = {0,1,2,3,2,1,2,3,4,2};
    /*  hundreds: C  CC  CCC  CD  D   DC  DCC  DCCC  CM */
    static const idx_t len_hund[10]  = {0,1,2,3,2,1,2,3,4,2};

    idx_t len = 0;

    /* thousands part – one 'M' per 1000 */
    idx_t thousands = x / 1000;
    len += thousands;

    /* hundreds, tens, ones */
    idx_t rest = x % 1000;
    idx_t h = rest / 100;
    idx_t t = (rest / 10) % 10;
    idx_t u = rest % 10;

    len += len_hund[h] + len_tens[t] + len_units[u];
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    idx_t count = 0;
    /* after 4000 there are no further terms; 5000 is a safe upper bound */
    const idx_t LIMIT = 5000;
    for (idx_t m = 1; m <= LIMIT; ++m)
    {
        if (roman_len(m) <= digit_len(m))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
    /* n is beyond the sequence length – undefined for the OEIS dataset */
    return 0;
}
