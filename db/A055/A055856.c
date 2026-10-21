/*
 * A055856  Susceptibility series H_4 for 2-dimensional Ising model (divided
 *          by 2).
 * https://oeis.org/A055856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4794*n^4 + 19194*n^2 + 3349 - 81*(-1)^n*(2*n^2 +5) +
 *            512*ChebyshevT(n, -1/2))/1728, with a(0)=1. ChebyshevT(n, -1/2)
 *            = 1 if n%3==0 else -1/2
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 198, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    unsigned __int128 NN = (unsigned __int128)n;
    unsigned __int128 n2 = NN * NN;
    unsigned __int128 n4 = n2 * n2;

    unsigned __int128 num = 0;
    num += (unsigned __int128)4794 * n4;
    num += (unsigned __int128)19194 * n2;
    num += 3349;

    unsigned __int128 two_n2_plus5 = (unsigned __int128)2 * n2 + 5;

    if ((n & 1) == 0) {
        num -= (unsigned __int128)81 * two_n2_plus5;  /* even n */
    } else {
        num += (unsigned __int128)81 * two_n2_plus5;  /* odd n */
    }

    if (n % 3 == 0) {
        num += 512;   /* ChebyshevT = 1  */
    } else {
        num -= 256;   /* ChebyshevT = -1/2 */
    }

    return (seq_t)(num / 1728);
}
