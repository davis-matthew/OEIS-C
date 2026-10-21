/*
 * A058186  Numbers (written in base 5) which appear the same when written in
 *          base 5 and base 10/2.
 * https://oeis.org/A058186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = nth decimal integer whose decimal representation uses only digits
   0,1,2,3,4 and whose all but the least significant digit are even
   (0,2,4).  The first 5 terms are 0,1,2,3,4.  For length ≥2 the most
   significant digit is restricted to {2,4}; all other intermediate digits
   are in {0,2,4} and the unit digit may be any 0–4.
   This is exactly the OEIS A058186.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n <= 5)
        return (seq_t)(n - 1);

    /* find the length L of the nth number */
    idx_t cum_prev = 5;           /* cumulative count up to length 1 */
    int    L       = 2;           /* current length being examined      */
    idx_t  pow3mid = 1;           /* 3^(L-2), number of choices for the
                                    middle even digits               */
    idx_t  group_size = 10 * pow3mid;  /* count of length-L numbers */

    while (n > cum_prev + group_size) {
        cum_prev += group_size;           /* add previous length group */
        ++L;                              /* move to next length      */
        pow3mid *= 3;                     /* update 3^(L-2)           */
        group_size = 10 * pow3mid;        /* new group size           */
    }

    /* offset within the current length group (0‑based) */
    idx_t offset = n - cum_prev - 1;

    /* decode offset into digits */
    int last_digit = (int)(offset % 5);          /* 0‑4 */
    idx_t even_index = offset / 5;              /* index over msd + middle even digits */

    /* first digit: 2 if even_index < pow3mid, else 4 */
    int first_digit = (even_index < pow3mid) ? 2 : 4;

    /* remaining middle even digits (L-2 of them) */
    int middle_len = L - 2;
    int middle[middle_len];
    idx_t mid_idx = even_index % pow3mid;       /* index within this msd group */

    for (int i = middle_len - 1; i >= 0; --i) {
        int d = (int)(mid_idx % 3);            /* 0,1,2 -> 0,2,4 */
        mid_idx /= 3;
        middle[i] = d * 2;
    }

    /* build decimal value */
    seq_t result = 0;
    result = result * (seq_t)10 + (seq_t)first_digit;
    for (int i = 0; i < middle_len; ++i)
        result = result * (seq_t)10 + (seq_t)middle[i];
    result = result * (seq_t)10 + (seq_t)last_digit;

    return result;
}
