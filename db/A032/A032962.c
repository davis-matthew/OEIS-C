/*
 * A032962  Numbers whose base-12 representation Sum_{i=0..m} d(i)*12^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest integer whose base‑12 representation
 *            has even digits (0,2,4,6,8,10) at all odd positions (counting
 *            from 0 at the most significant digit)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t odd_digits[6]  = {0,2,4,6,8,10};
static const uint8_t even_digits[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

seq_t a(idx_t n)
{
    /* find the length of the base‑12 representation of the n‑th term */
    idx_t len = 1;
    unsigned __int128 cumulative = 0;

    while (1) {
        unsigned __int128 count = 11;                   /* leading digit: 1‑11 */
        for (idx_t i = 1; i < len; ++i) {
            count *= (i & 1) ? 6 : 12;                  /* 6 for odd pos, 12 for even */
        }
        if (cumulative + count >= n) {
            break;
        }
        cumulative += count;
        ++len;
    }

    /* offset within numbers of this length (0‑based) */
    idx_t offset = n - (idx_t)cumulative - 1ULL;

    /* radices for positions 0 .. len-1 (0‑based from the most significant) */
    unsigned int rad[64];
    rad[0] = 11;
    for (idx_t i = 1; i < len; ++i) {
        rad[i] = (i & 1) ? 6 : 12;
    }

    /* suffix products of radices: sfx[i] = product rad[i]..rad[len-1] */
    unsigned __int128 sfx[65];
    sfx[len] = 1;
    for (idx_t i = len; i-- > 0; ) {
        sfx[i] = rad[i] * sfx[i+1];
    }

    /* build the number digit by digit */
    seq_t value = 0;
    for (idx_t pos = 0; pos < len; ++pos) {
        unsigned __int128 m = sfx[pos+1];

        /* digit index within the allowed digits at this position */
        uint64_t digit_index = (uint64_t)((( unsigned __int128)offset) / m);
        offset = (idx_t)((( unsigned __int128)offset) % m);

        uint8_t digit;
        if (pos == 0) {                 /* leading digit: 1‑11 */
            digit = 1 + (uint8_t)digit_index;
        } else if (pos & 1) {            /* odd position: even digits only */
            digit = odd_digits[digit_index];
        } else {                         /* even position: any digit 0‑11 */
            digit = even_digits[digit_index];
        }

        value = value * 12 + digit;
    }

    return value;
}
