/*
 * A078194  In the following triangle the n-th row contains n n-digit (or
 *          (n-1)-digit) numbers whose concatenation (with a 0 prefixed for
 *          (n-1)-digit numbers) gives a substring of the cyclic concatenation
 *          of 1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,...: 1; 12 34; 123
 *          456 789; 1234 5678 9012 3456; 12345 67...
 * https://oeis.org/A078194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the k-th number in row r, where r is the unique integer *
 *            satisfying r*(r-1)/2 < n <= r*(r+1)/2, and k = n - r*(r-1)/2. *
 *            Row r consists of the first r² digits of the infinite cyclic
 *            sequence * 1,2,3,4,5,6,7,8,9,0,1,2,… partitioned into r
 *            consecutive blocks of * length r, each interpreted...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 55, u64 n <= 210, u128 n <= 743,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r-1)/2 < n <= r*(r+1)/2.
       Use a simple accumulating loop; r is about sqrt(2*n). */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;   /* sum = 1+2+…+r = r*(r+1)/2 */
    }

    /* Number of terms preceding this row */
    idx_t prev = sum - r;          /* = (r-1)*r/2 */

    /* Position (1‑based) of the desired number within the row */
    idx_t k = n - prev;            /* 1 ≤ k ≤ r */

    /* Starting position (1‑based) of the k‑th block of length r */
    idx_t start = 1 + (k - 1) * r; /* first digit of the block */

    /* Build the number by reading r consecutive digits of the cycle */
    seq_t value = 0;
    for (idx_t i = 0; i < r; ++i) {
        idx_t pos   = start + i;                      /* global digit position */
        unsigned digit = (unsigned)(pos % 10ULL);     /* sequence: 1,2,…,9,0,1,… */
        value = value * 10 + (seq_t)digit;
    }

    return value;
}
