/*
 * A183908  Number of nondecreasing arrangements of n+2 numbers in 0..5 with
 *          each number being the sum mod 6 of two others.
 * https://oeis.org/A183908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 25*n^4 + 245*n^3 - 145*n^2 – 2766*n + 5160)/120
 *            for n > 2 with initial terms a(1)=2, a(2)=8
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)8;

    /* use 128‑bit intermediate to avoid overflow (works for GNU/C99/C++17) */
    __int128 N;
    __int128 n128 = (unsigned long long)n;          /* promote to 128 bits */
    __int128 n2   = n128 * n128;
    __int128 n3   = n2   * n128;
    __int128 n4   = n3   * n128;
    __int128 n5   = n4   * n128;

    N = n5
      + (__int128)25  * n4
      + (__int128)245 * n3
      - (__int128)145 * n2
      - (__int128)2766 * n128
      + (__int128)5160;

    uint64_t val = (uint64_t)(N / 120);   /* result fits in 64 bits for all n of interest */
    return (seq_t)val;
}
