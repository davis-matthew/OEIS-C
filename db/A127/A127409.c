/*
 * A127409  Negative value of coefficient of x^(n-4) in the characteristic
 *          polynomial of a certain n X n integer circulant matrix.
 * https://oeis.org/A127409
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^8 - 5*n^7 - 45*n^6 + 125*n^5 - 78*n^4) / 240
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (identical to A127409, valid for n ≥ 4, returns 0 for n < 4) */
seq_t a(idx_t n)
{
    /* The sequence is undefined for indices below the offset 4. */
    if (n < 4)
        return 0;

    /* Compute the polynomial using 128‑bit signed arithmetic
       to avoid overflow in intermediate subtractions.              */
    __int128 N  = (__int128)n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;
    __int128 n7 = n6 * N;
    __int128 n8 = n7 * N;

    __int128 numerator =
          3 * n8
        - 5 * n7
        - 45 * n6
        + 125 * n5
        - 78 * n4;

    numerator /= 240;   /* 240 = 2·5!  – division is exact */

    return (seq_t)numerator;
}
