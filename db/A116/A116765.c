/*
 * A116765  Number of permutations of length n which avoid the patterns 1234,
 *          2431, 4312.
 * https://oeis.org/A116765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 1; for n >= 2, a(n) = (n^5 + 75*n^4 - 715*n^3 +
 *            2685*n^2 - 4446*n + 2880) / 120
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
    {
        return (seq_t)1;
    }

    /* Use an intermediate 128‑bit type for safety when seq_t is 64‑bit.
       Multiplication and subtraction are performed in unsigned __int128
       and the final result is cast back to seq_t. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;

    unsigned __int128 num = n5;
    num += (unsigned __int128)75  * n4;
    num -= (unsigned __int128)715 * n3;
    num += (unsigned __int128)2685 * n2;
    num -= (unsigned __int128)4446 * N;
    num += 2880;

    num /= 120;
    return (seq_t)num;
}
