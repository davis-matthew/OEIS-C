/*
 * A173651  Triangle T(n,m) read by rows: numerator of 1/(n-m)^2-1/m^2, or -1
 *          if m=0.
 * https://oeis.org/A173651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, frac, easy
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 113, i32 n <= 113, i64 n <= 113,
 *            i128 n <= 113, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A173651  
 *  a(n) = numerator of the reduced fraction 1/(r-m)^2 - 1/m^2,  
 *  where r = minimal integer with r*(r+1)/2 ≥ n  
 *        m = n - r*(r-1)/2 - 1   (0 ≤ m < r).  
 *  If m = 0 the term is defined to be -1. */

#include "../utils/gcd.h"

static inline idx_t row_from_index(idx_t n) {
    /* Find the smallest r with r(r+1)/2 ≥ n. */
    idx_t low = 1, high = n;          /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the absolute value of a seq_t (assume overflow does not occur). */
static inline seq_t abs_seq(seq_t x) {
    return x < 0 ? -x : x;
}

/* Compute T(r,m): numerator after reducing 1/(r-m)^2 - 1/m^2. */
static inline seq_t tri_entry(idx_t r, idx_t m) {
    if (m == 0)                          /* special definition */
        return (seq_t)-1;
    idx_t a = r - m;                     /* r-m   > 0         */
    idx_t b = m;                         /* m     > 0         */
    seq_t aa = (seq_t)a * (seq_t)a;      /* a^2                 */
    seq_t bb = (seq_t)b * (seq_t)b;      /* b^2                 */
    seq_t num = bb - aa;                 /* b^2 - a^2           */
    seq_t denom = aa * bb;               /* a^2 * b^2           */
    seq_t abs_num = abs_seq(num);
    seq_t g   = gcd(abs_num, denom);     /* gcd is non‑negative    */
    return num / g;                      /* keeps the sign of num  */
}

seq_t a(idx_t n) {
    idx_t r = row_from_index(n);
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t m = n - prev_tri - 1;
    return tri_entry(r, m);
}
