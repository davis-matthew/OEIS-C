/*
 * A110651  n^2 followed by n^4 followed by n^3 followed by n.
 * https://oeis.org/A110651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 1021, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A110651:  n^2 followed by n^4 followed by n^3 followed by n.
    a(n) = floor((n+3)/4) ^ e, where e = 2,4,3,1 for (n-1) mod 4 = 0,1,2,3 respectively. */

seq_t a(idx_t n)
{
    /* base = floor((n+3)/4)   (unsigned integer division) */
    idx_t base = (n + 3) / 4;

    /* position inside each block of four */
    idx_t t = (n - 1) % 4;

    /* exponent depending on that position */
    static const idx_t exp_map[4] = {2, 4, 3, 1};
    idx_t exp = exp_map[t];

    /* compute base^exp in seq_t */
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;

    while (exp > 0) {
        result *= b;
        exp--;
    }
    return result;
}
