/*
 * A068094  Number of n-digit triangular numbers.
 * https://oeis.org/A068094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   Count of n‑digit triangular numbers a(n) = #{ k ≥ 1 | 10^(n-1) ≤
 *            k(k+1)/2 ≤ 10^n – 1 }
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   _pow10_128, _sqrt128, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"           /* arbitrary‑precision pow10()         */
#include "../utils/floor_sqrt.h"      /* floor_sqrt() for arbitrary seq_t   */
#include "../utils/_pow10_128.h"
#include "../utils/_sqrt128.h"

/*--- Main sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for out of range */

    if (sizeof(seq_t) <= 8) {            /* 64‑bit or smaller -- use 128‑bit tmp */
        unsigned __int128 pow_n_minus1 = _pow10_128(n - 1);
        unsigned __int128 pow_n       = pow_n_minus1 * 10;          /* 10^n   */

        /* L = 10^(n-1)  =>  D1 = 8*L + 1  */
        unsigned __int128 D1 = pow_n_minus1 * 8 + 1;
        unsigned __int128 s1 = _sqrt128(D1);          /* floor(√D1)  */
        unsigned __int128 k_min;
        if (s1 * s1 == D1)                      /* perfect square */
            k_min = (s1 - 1) / 2;
        else
            k_min = (s1 + 1) / 2;                /* ceil of (-1+√D1)/2 */

        /* R = 10^n – 1  =>  D2 = 8*R + 1 = 8*10^n – 7  */
        unsigned __int128 D2 = pow_n * 8 - 7;
        unsigned __int128 s2 = _sqrt128(D2);          /* floor(√D2)  */
        unsigned __int128 k_max = (s2 - 1) / 2;        /* floor of (-1+√D2)/2 */

        if (k_max < k_min) return (seq_t)0;

        return (seq_t)(k_max - k_min + 1);
    }

    /*---  arbitrary‑precision seq_t ------------------------------------------*/
    seq_t L = pow10(n - 1);                    /* 10^(n-1)  */
    seq_t R = pow10(n) - (seq_t)1;             /* 10^n – 1  */

    seq_t D1 = (seq_t)8 * L + (seq_t)1;        /* 8*L + 1   */
    seq_t s1 = floor_sqrt(D1);                 /* floor(√D1) */
    seq_t k_min = (s1 * s1 == D1) ? (s1 - (seq_t)1) / (seq_t)2
                                   : (s1 + (seq_t)1) / (seq_t)2;

    seq_t D2 = (seq_t)8 * R + (seq_t)1;        /* 8*R + 1   */
    seq_t s2 = floor_sqrt(D2);                 /* floor(√D2) */
    seq_t k_max = (s2 - (seq_t)1) / (seq_t)2;   /* floor(-1+√D2)/2 */

    if (k_max < k_min) return (seq_t)0;
    return k_max - k_min + (seq_t)1;
}
