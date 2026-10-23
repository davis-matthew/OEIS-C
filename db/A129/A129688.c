/*
 * A129688  A129686 * A128174.
 * https://oeis.org/A129688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = { 1, if r=j ;
 *                  0, if r=j+1 ;
 *                  2, if r-j>=2 and (r-j) even ;
 *                  0, otherwise } ,
 * where r is the row number of the triangle and j the column number.
 * This defines the triangle shown in the OEIS entry for A129688.
 */
#include <stdint.h>

/* find the row r such that r*(r+1)/2 >= n */
static idx_t find_row(idx_t n) {
    idx_t low = 1, high = n;          // triangular(high) >= high >= n
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* determine row r and column j (1‑based) */
    idx_t r = find_row(n);
    idx_t r_minus_one = r - 1;
    unsigned __int128 t_prev128 = (unsigned __int128)r_minus_one * r / 2;
    idx_t t_prev = (idx_t)t_prev128;          /* triangular(r-1) */
    idx_t pos = n - t_prev;                  /* column index j, 1‑based */
    idx_t d = r - pos;                       /* difference r-j */

    /* compute the value according to the pattern */
    if (d == 0)
        return (seq_t)1;
    if (d == 1)
        return (seq_t)0;
    if ((d & 1) == 0)                       /* even difference >= 2 */
        return (seq_t)2;
    return (seq_t)0;
}
