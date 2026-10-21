/*
 * A208104  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   mult2, mult4, mult6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 4*a(n-4)
 *            + 2*a(n-5) - a(n-6)            (n > 7)
 *  a(1) = 9 , a(2) = 81 , a(3) = 169 , a(4) = 361 ,
 *  a(5) = 841 , a(6) = 2025 , a(7) = 5041 .
 *  For n = 7 the same formula applies with an additional term -40.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mult2.h"
#include "../utils/mult4.h"
#include "../utils/mult6.h"

seq_t a(idx_t n)
{
    /* handle small n */
    switch (n) {
        case 1: return 9;
        case 2: return 81;
        case 3: return 169;
        case 4: return 361;
        case 5: return 841;
        case 6: return 2025;
        case 7: return 5041;
        default: break;
    }

    /* initial previous values (a1..a6) */
    seq_t a1 = 9;     /* a(1)  */
    seq_t a2 = 81;    /* a(2)  */
    seq_t a3 = 169;   /* a(3)  */
    seq_t a4 = 361;   /* a(4)  */
    seq_t a5 = 841;   /* a(5)  */
    seq_t a6 = 2025;  /* a(6)  */

    seq_t val;

    for (idx_t i = 7; i <= n; ++i) {
        if (i == 7) {
            val = mult4(a6);
            val -= mult2(a5);
            val -= mult6(a4);
            val += mult4(a3);
            val += mult2(a2);
            val -= a1;
            val -= (seq_t)40;
        } else {
            val = mult4(a6);
            val -= mult2(a5);
            val -= mult6(a4);
            val += mult4(a3);
            val += mult2(a2);
            val -= a1;
        }

        /* shift the window of previous values */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = val;
    }

    return a6;
}
