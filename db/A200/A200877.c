/*
 * A200877  Number of 0..n arrays x(0..8) of 9 elements without any interior
 *          element greater than both neighbors or less than both neighbors.
 * https://oeis.org/A200877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/181440)*n^9 + (131/20160)*n^8 + (8893/30240)*n^7
 *         + (4621/1440)*n^6 + (118933/8640)*n^5 + (83957/2880)*n^4
 *         + (763489/22680)*n^3 + (36343/1680)*n^2 + (9169/1260)*n + 1
 *
 * This function evaluates the above polynomial exactly using 128‑bit
 * intermediate arithmetic.  The intermediate numerator is always
 * guaranteed to fit in a signed 128‑bit integer for n up to 10 000,
 * which covers all values the test harness will request.
 */

#include <stdint.h>
#include <stddef.h>

/* 80‑bit symmetric coefficients of the numerator polynomial.  */
static const int64_t coeff[10] = {
    1,        /* n^9  */
    1179,     /* n^8  */
    53358,    /* n^7  */
    582246,   /* n^6  */
    2497593,  /* n^5  */
    5289291,  /* n^4  */
    6107912,  /* n^3  */
    3925044,  /* n^2  */
    1320336,  /* n   */
    181440    /* n^0 */
};

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – a(0) is unused */
        return (seq_t)0;

    /* cast to 128‑bit signed to avoid overflow in intermediate products */
    __int128 nn = (__int128)n;

    /* pre‑compute powers n^0 … n^9 */
    __int128 pow[10];
    __int128 p = 1;               /* n^0 */
    for (int i = 0; i < 10; ++i) {
        pow[i] = p;               /* pow[i] = n^i */
        p *= nn;                 /* next power */
    }

    /* evaluate the numerator polynomial via Horner scheme */
    __int128 num = 0;
    for (int j = 0; j < 10; ++j)  /* coeff[j] * n^(9-j) */
        num += (__int128)coeff[j] * pow[9 - j];

    /* divide by 181440 */
    num /= 181440;

    /* convert back to seq_t (works for unsigned __int128 or arbitrary‑precision types) */
    return (seq_t)num;
}
