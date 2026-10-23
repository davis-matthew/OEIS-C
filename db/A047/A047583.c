/*
 * A047583  Numbers that are congruent to {0, 3, 5, 6, 7} mod 8.
 * https://oeis.org/A047583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/5)*8 + [0,3,5,6,7][(n-1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0, a(2)=3, a(3)=5, a(4)=6, a(5)=7, a(6)=8, ... */

seq_t a(idx_t n)
{
    /* For safety, handle the undefined case n==0 */
    if (n == 0)
        return 0;

    static const seq_t offsets[5] = {0, 3, 5, 6, 7};

    /* Zero‑based index within the 5‑term pattern */
    idx_t q = (n - 1) / 5;
    idx_t r = (n - 1) % 5;

    /* Compute 8*q in a wide enough type then cast to seq_t */
    seq_t result = (seq_t)(q * 8) + offsets[r];
    return result;
}
