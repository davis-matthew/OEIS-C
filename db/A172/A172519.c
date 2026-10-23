/*
 * A172519  Number of ways to place 4 nonattacking queens on an n X n toroidal
 *          board.
 * https://oeis.org/A172519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8/24 - n^7 + 245*n^6/24 - 113*n^5/2 + 2843*n^4/16 -
 *            593*n^3/2 + 4757*n^2/24) + (n^6/8 - 5*n^5/2 + 305*n^4/16 -
 *            129*n^3/2 + 629*n^2/8)*(-1)^n + 8*n^2*cos(2*Pi*n/3)/3 +
 *            9*n^2*cos(Pi*n/2)/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 26, u64 n <= 383, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    const unsigned __int128 N   = (unsigned __int128)n;
    const unsigned __int128 n2  = N * N;
    const unsigned __int128 n3  = n2 * N;
    const unsigned __int128 n4  = n2 * n2;
    const unsigned __int128 n5  = n4 * N;
    const unsigned __int128 n6  = n3 * n3;
    const unsigned __int128 n7  = n6 * N;
    const unsigned __int128 n8  = n7 * N;

    unsigned __int128 pos = 0;
    unsigned __int128 neg = 0;

    /* Base polynomial part: (coefficients multiplied by 48). */
    pos += (unsigned __int128)2   * n8;    /* 2 * n^8                 */
    neg += (unsigned __int128)48  * n7;    /* -48 * n^7                */
    pos += (unsigned __int128)490 * n6;    /* 490 * n^6                */
    neg += (unsigned __int128)2712 * n5;   /* -2712 * n^5              */
    pos += (unsigned __int128)8529 * n4;   /* 8529 * n^4               */
    neg += (unsigned __int128)14232 * n3;  /* -14232 * n^3             */
    pos += (unsigned __int128)9514 * n2;   /* 9514 * n^2               */

    /* Parity-dependent part multiplied by 48. */
    if ((n & 1ULL) == 0ULL) {              /* n even : (-1)^n = 1      */
        pos += (unsigned __int128)6   * n6;
        neg += (unsigned __int128)120 * n5;
        pos += (unsigned __int128)915 * n4;
        neg += (unsigned __int128)3096 * n3;
        pos += (unsigned __int128)3774 * n2;
    } else {                               /* n odd  : (-1)^n = -1    */
        neg += (unsigned __int128)6   * n6;
        pos += (unsigned __int128)120 * n5;
        neg += (unsigned __int128)915 * n4;
        pos += (unsigned __int128)3096 * n3;
        neg += (unsigned __int128)3774 * n2;
    }

    /* Cos(2*Pi*n/3) term: +128*n^2 for n%3==0, -64*n^2 otherwise. */
    switch (n % 3ULL) {
        case 0: pos += (unsigned __int128)128 * n2; break;
        default: neg += (unsigned __int128)64  * n2; break;
    }

    /* Cos(Pi*n/2) term: +216*n^2 for n%4==0, -216*n^2 for n%4==2. */
    switch (n % 4ULL) {
        case 0: pos += (unsigned __int128)216 * n2; break;
        case 2: neg += (unsigned __int128)216 * n2; break;
        default: break;
    }

    /* The numerator is (pos - neg); divide by 48 to recover a(n). */
    const unsigned __int128 numerator = pos - neg;
    return (seq_t)(numerator / (unsigned __int128)48);
}
