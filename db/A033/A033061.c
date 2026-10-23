/*
 * A033061  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer whose decimal digits satisfy
 * that for every odd i, the digit at position m-i (where d(m) is the most
 * significant digit) is odd.  The function computes this by brute‑force
 * searching the natural numbers until the n‑th qualifying integer is found. */
 
#include <stdbool.h>
 
/* Helper: Return true if the decimal representation of x satisfies the
 * required parity condition.  The most significant digit has index m. */
static bool is_valid(seq_t x) {
    /* First determine the number of digits (len = m+1). */
    int len = 0;
    seq_t y = x;
    while (y > 0) {
        len++;
        y /= 10;
    }
    if (len == 0) return false;      /* x == 0 is not part of the sequence */
    
    /* For an even length (len even), digits at even indices must be odd.
       For an odd length, digits at odd indices must be odd. */
    int odd_needed = (len % 2 == 0) ? 0 : 1;  /* 0 for even indices, 1 for odd */
    
    int j = 0;
    seq_t t = x;
    while (t > 0) {
        int digit = (int)(t % 10);          /* LSB digit */
        if (((j & 1) == odd_needed) && ((digit & 1) == 0)) {
            return false;                  /* required odd digit is even */
        }
        t /= 10;
        j++;
    }
    return true;
}
 
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* offset is 1; below 1 undefined */
    idx_t count = 0;
    seq_t num = 1;
    while (true) {
        if (is_valid(num)) {
            count++;
            if (count == n) return num;
        }
        num++;
    }
}
