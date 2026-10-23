/*
 * A047466  Numbers that are congruent to {0, 1, 2, 4} mod 8.
 * https://oeis.org/A047466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)/4)*8 + {0,1,2,4}[ (n-1) mod 4 ]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _add[4] = {0, 1, 2, 4};

seq_t a(idx_t n)
{
    /* The offset is 1, so n >= 1.  For n == 0 the value is undefined. */
    idx_t n_minus_one = n - 1;
    idx_t k = n_minus_one >> 2;          /* floor((n-1)/4) */
    unsigned r = (unsigned)(n_minus_one & 3u); /* (n-1) mod 4 */

    seq_t res = ((seq_t)k << 3) + _add[r];   /* 8*k + offset */
    return res;
}
