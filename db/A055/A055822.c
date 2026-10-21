/*
 * A055822  a(n) = T(n, n-5), array T as in A055818.
 * https://oeis.org/A055822
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (480 +524*n -250*n^2 -45*n^3 +10*n^4 + n^5)/120 for n>5,
 *            a(5)=1
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 218, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;          /* undefined for n < 5, behavior is irrelevant */
    if (n == 5)
        return 1;          /* a(5) = 1 */

    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       When seq_t is 64‑bit the final value still fits, and when seq_t
       is 128‑bit or arbitrary‑precision the cast is safe. */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;               /* n^2 */
    unsigned __int128 n3  = n2 * nn;               /* n^3 */
    unsigned __int128 n4  = n3 * nn;               /* n^4 */
    unsigned __int128 n5  = n4 * nn;               /* n^5 */

    unsigned __int128 num = (unsigned __int128)480;
    num += (unsigned __int128)524 * nn;
    num -= (unsigned __int128)250 * n2;
    num -= (unsigned __int128)45  * n3;
    num += (unsigned __int128)10  * n4;
    num += n5;
    num /= 120;                     /* division is exact */

    return (seq_t)num;
}
