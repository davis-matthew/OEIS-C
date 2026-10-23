/*
 * A184223  Half the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          diagonal sum differing from its antidiagonal sum by more than 3.
 * https://oeis.org/A184223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A184223
 *
 *  Recurrence:  a(1)=15, a(2)=57, a(3)=180
 *                a(n)=3*a(n‑1)+3*a(n‑2)-6*a(n‑3)   (n≥4)
 *
 *  Sequence: 15, 57, 180, 621, 2061, 6966, …
 *
 *  This implementation is valid for any seq_t that supports the usual
 *  arithmetic operators (+, -, *, /, <, >, ==, etc.) and any idx_t
 *  that is an unsigned 64‑bit integer.  No dynamic allocation is
 *  required and therefore the function works both for plain integer
 *  types and for arbitrary‑precision big integer types (which may be
 *  non‑trivial and cannot be allocated with malloc/realloc).
 *
 *  The function operates in O(n) time and O(1) auxiliary space.
 *
 *  External linkage only:  seq_t a(idx_t n);
 */
#include <stdbool.h>   /* for bool type */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1, return 0 */

    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)57;
    if (n == 3) return (seq_t)180;

    seq_t a_nm3 = (seq_t)15;  /* a(n-3)  */
    seq_t a_nm2 = (seq_t)57;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)180; /* a(n-1)  */
    seq_t a_n   = (seq_t)0;   /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(n) = 3*a(n-1) + 3*a(n-2) - 6*a(n-3) */
        a_n = (seq_t)3 * a_nm1 + (seq_t)3 * a_nm2 - (seq_t)6 * a_nm3;

        /* slide the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
