/*
 * A034123  Fractional part of cube root of a(n) starts with digit 7.
 * https://oeis.org/A034123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th integer m such that the fractional part of the
 *            real cube root of m begins with digit 7, i.e. 0.7 ≤ {m^(1/3)} <
 *            0.8. This is computed by iterating over k = ⌊m^(1/3)⌋ and
 *            finding the integer interval [ceil((k+0,7)^3),
 *            ceil((k+0,8)^3)−1] that satisfies the condition
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) is undefined; return 0 safely */

    idx_t count = 0;        /* number of terms found so far */
    idx_t k = 1;            /* current floor cube root */

    while (1) {
        /* compute (10k+7)^3 and (10k+8)^3 using 128‑bit arithmetic */
        unsigned __int128 a10 = 10 * (unsigned __int128)k + 7;
        unsigned __int128 lowN = a10 * a10 * a10;

        unsigned __int128 b10 = 10 * (unsigned __int128)k + 8;
        unsigned __int128 highN = b10 * b10 * b10 - 1;

        /* ceil(lowN/1000)  and  floor((highN)/1000) */
        unsigned __int128 lower = (lowN + 999) / 1000;
        unsigned __int128 upper = highN / 1000;

        /* number of integers in this interval */
        idx_t sz = (upper >= lower ? (idx_t)(upper - lower + 1) : 0);

        if (count + sz < n) {
            count += sz;
            ++k;
            continue;
        }

        /* the desired term lies within this interval */
        unsigned __int128 offset = (unsigned __int128)(n - 1 - count);
        unsigned __int128 val = lower + offset;
        return (seq_t)val;
    }
}
