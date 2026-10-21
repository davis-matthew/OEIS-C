/*
 * A226096  Squares with doubled (4*n+2)^2.
 * https://oeis.org/A226096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*(16*floor(n/5)^2 + 8*floor(n/5) + 1) - (11 +
 *            24*floor(n/5))*(n - 5*floor(n/5))^4 + 2*(49 + 104*floor(n/5))*(n
 *            - 5*floor(n/5))^3 - 23*(11 + 24*floor(n/5))*(n - 5*floor(n/5))^2
 *            + 2*(119 + 280*floor(n/5))*(n - 5*floor(n/5))) / 24
 * Range:     u8 n <= 3, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* split n into m = floor(n/5) and r = n mod 5 */
    idx_t m_idx = n / 5;
    idx_t r_idx = n % 5;
    seq_t m   = (seq_t)m_idx;
    seq_t r   = (seq_t)r_idx;

    /* compute r powers (r <= 4, can be stored in seq_t safely) */
    seq_t r2 = r * r;
    seq_t r3 = r2 * r;
    seq_t r4 = r3 * r;

    /* part1 = 24*(16*m^2 + 8*m + 1) */
    seq_t part1 = 16 * m * m;
    part1 += 8 * m;
    part1 += (seq_t)1;
    part1 *= (seq_t)24;

    /* other parts using m and r */
    seq_t part2 = (seq_t)(11) + (seq_t)(24) * m;
    part2 *= r4;

    seq_t part3 = (seq_t)(49) + (seq_t)(104) * m;
    part3 *= (seq_t)2 * r3;

    seq_t part4 = (seq_t)(11) + (seq_t)(24) * m;
    part4 *= (seq_t)23 * r2;

    seq_t part5 = (seq_t)(119) + (seq_t)(280) * m;
    part5 *= (seq_t)2 * r;

    /* combine according to the formula */
    seq_t numer = part1 - part2 + part3 - part4 + part5;

    /* divide by 24 (exact) */
    return numer / (seq_t)24;
}
