/*
 * A183910  Number of nondecreasing arrangements of n+2 numbers in 0..7 with
 *          each number being the sum mod 8 of two others.
 * https://oeis.org/A183910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 42*n^6 + 742*n^5 + 7140*n^4 - 9191*n^3 -
 *            223902*n^2 * + 714048*n - 418320) / 5040 for n > 3 * Initial
 *            values: a(1)=2, a(2)=5, a(3)=40
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)40;

    /* Compute the polynomial using 128‑bit arithmetic for safety */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 num = 0;

    num += N * N * N * N * N * N * N;                     // n^7
    num += (unsigned __int128)42 * N * N * N * N * N * N;   // 42*n^6
    num += (unsigned __int128)742 * N * N * N * N * N;     // 742*n^5
    num += (unsigned __int128)7140 * N * N * N * N;        // 7140*n^4
    num -= (unsigned __int128)9191 * N * N * N;            // -9191*n^3
    num -= (unsigned __int128)223902 * N * N;             // -223902*n^2
    num += (unsigned __int128)714048 * N;                 // +714048*n
    num -= (unsigned __int128)418320;                     // -418320

    return (seq_t)(num / (unsigned __int128)5040);
}
