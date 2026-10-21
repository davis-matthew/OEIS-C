/*
 * A210766  Number of 8-hoops with n symbols and no a-rooted trees.
 * https://oeis.org/A210766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 8*n^7 + 36*n^6 - 92*n^5 + 155*n^4 - 148*n^3 * +
 *            88*n^2 - 16*n) / 16 * (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 363, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined through a degree‑8 polynomial.
       We evaluate it with Horner’s rule using a signed 128‑bit
       intermediate.  For n ≤ 10 000 the value fits comfortably in
       128 bits, and the division is exact. */
    if (n == 0)
        return 0;          /* undefined for offset 1, safe return */

    __int128_t x = ( __int128_t )n;
    __int128_t res = 1;        /* coefficient of n^8 */

    res = res * x + ( __int128_t )-8;   /* a7 = -8   */
    res = res * x + ( __int128_t )36;   /* a6 =  36  */
    res = res * x + ( __int128_t )-92;  /* a5 = -92  */
    res = res * x + ( __int128_t )155;  /* a4 = 155  */
    res = res * x + ( __int128_t )-148; /* a3 = -148 */
    res = res * x + ( __int128_t )88;   /* a2 =  88  */
    res = res * x + ( __int128_t )-16;  /* a1 = -16  */
    res = res * x + ( __int128_t )0;    /* a0 =   0  */

    res /= 16;                  /* division by 16 is exact */
    return (seq_t)res;
}
