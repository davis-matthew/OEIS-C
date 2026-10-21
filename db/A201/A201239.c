/*
 * A201239  Number of ways to place 5 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n^8 - 50n^6 + 995n^4 - 9370n^2 + 35424)/120, n>=6
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n<6 the values are a(1)=0, a(2)=0, a(3)=0, a(4)=128, a(5)=6745 */

#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly */
    if (n < 6) {
        switch ((size_t)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)0;
            case 4: return (seq_t)128;
            case 5: return (seq_t)6745;
            default: return (seq_t)0; /* unreachable, keeps compiler happy */
        }
    }

    /* Use 128‑bit intermediate arithmetic to avoid overflow on 64‑bit builds */
    unsigned __int128 NN  = (unsigned __int128)n;
    unsigned __int128 n2  = NN * NN;          /* n^2   */
    unsigned __int128 n4  = n2 * n2;          /* n^4   */
    unsigned __int128 n6  = n4 * n2;          /* n^6   */
    unsigned __int128 n8  = n4 * n4;          /* n^8   */

    unsigned __int128 poly = n8;
    poly -= (unsigned __int128)50 * n6;
    poly += (unsigned __int128)995 * n4;
    poly -= (unsigned __int128)9370 * n2;
    poly += (unsigned __int128)35424;

    unsigned __int128 num = n2 * poly;        /* n^2 * poly */
    return (seq_t)(num / 120);
}
