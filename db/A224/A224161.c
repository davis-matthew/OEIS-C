/*
 * A224161  Number of 5Xn 0..1 arrays with diagonals and rows unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3628800)*n^10 – (1/241920)*n^9 + (13/60480)*n^8 –
 *            (1/2688)*n^7 + (15793/172800)*n^6 – (10013/11520)*n^5 +
 *            (3158927/362880)*n^4 – (461413/12096)*n^3 + (13189843/50400)*n^2
 *            – (797567/840)*n + 2301 for n > 6. For n ≤ 6 the values are
 *            given explicitly. The code uses a 128‑bit intermediate t...
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    /* Explicit values for the first 6 indices. */
    switch (n) {
        case 1: return (seq_t)32;
        case 2: return (seq_t)324;
        case 3: return (seq_t)900;
        case 4: return (seq_t)1783;
        case 5: return (seq_t)3469;
        case 6: return (seq_t)6786;
        default: break;   /* n > 6 proceeds below */
    }

#if defined(__SIZEOF_INT128__)
    /* Use a 128‑bit integer for the intermediate computation. */
    typedef __int128 int128_t;
    int128_t N = (int128_t)n;

    /* Pre‑compute powers n^10 … n^0. */
    int128_t pow[11];
    pow[10] = 1;                       /* n^0 */
    for (int i = 9; i >= 0; --i)
        pow[i] = pow[i + 1] * N;       /* n^(10-i) */

    /* Integer coefficients of the polynomial multiplied by 3628800. */
    const int128_t coeff[11] = {
        1,               /* n^10 */
       -15,              /* n^9 */
        780,              /* n^8 */
       -1350,             /* n^7 */
        331653,           /* n^6 */
       -3154095,          /* n^5 */
        31589270,         /* n^4 */
       -138423900,        /* n^3 */
        949668696,        /* n^2 */
       -3445489440,       /* n^1 */
        8349868800        /* n^0 */
    };

    int128_t num = 0;
    for (int i = 0; i < 11; ++i)
        num += coeff[i] * pow[i];

    int128_t result = num / 3628800;
    return (seq_t)result;
#else
    /* Fallback: compute entirely with seq_t (may overflow for small types). */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * n1;
    seq_t n6 = n4 * n2;
    seq_t n7 = n6 * n1;
    seq_t n8 = n4 * n4;
    seq_t n9 = n8 * n1;
    seq_t n10 = n9 * n1;

    seq_t num = 0;
    num += n10;
    num -= (seq_t)15 * n9;
    num += (seq_t)780 * n8;
    num -= (seq_t)1350 * n7;
    num += (seq_t)331653 * n6;
    num -= (seq_t)3154095 * n5;
    num += (seq_t)31589270 * n4;
    num -= (seq_t)138423900 * n3;
    num += (seq_t)949668696 * n2;
    num -= (seq_t)3445489440 * n1;
    num += (seq_t)8349868800;
    return num / (seq_t)3628800;
#endif
}
