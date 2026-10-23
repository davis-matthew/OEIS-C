/*
 * A147534  a(n) is congruent to (1,1,2) mod 3.
 * https://oeis.org/A147534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + 3, for n > 3 Initial values: a(1) = 1, a(2) = 1,
 *            a(3) = 2 This computes the terms congruent to (1,1,2) modulo 3
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly when n ≤ 3. */
    static const seq_t base[4] = {0, 1, 1, 2};   /* base[1] = a(1), base[2] = a(2), base[3] = a(3) */

    /* Compute the position modulo 3.  We use 3 for 0 to align with the base array. */
    idx_t r = n % 3;
    if (r == 0)
        r = 3;

    /* a(n) = a(r) + (n - r)  because  a(r) + 3 * ((n - r) / 3) = a(r) + (n - r) */
    return (seq_t)(n - r) + base[r];
}
