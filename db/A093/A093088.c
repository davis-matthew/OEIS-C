/*
 * A093088  "Fibonacci-like in digits": start with a(1)=1, a(2)=4; repeatedly
 *          adjoin digits of sum of previous two terms.
 * https://oeis.org/A093088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Start with a(1)=1, a(2)=4.  For i=1,2,… compute s=a(i)+a(i+1)
   and append the decimal digits of s to the list.  This generates the
   sequence A093088. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Allocate space slightly larger than n to accommodate the final
       possible pair of digits when a two‑digit sum is appended. */
    idx_t cap = n + 10;
    seq_t *a = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!a) return 0;   /* fallback on allocation failure */

    a[1] = (seq_t)1;
    a[2] = (seq_t)4;
    idx_t len = 2;          /* current length of the sequence */
    idx_t i   = 1;          /* current pair index */

    while (len < n) {
        /* Compute the sum of the current adjacent pair. */
        seq_t sum = a[i] + a[i + 1];

        /* Append the decimal digits of the sum. */
        if (sum >= 10) {
            a[++len] = sum / 10;   /* most significant digit */
            a[++len] = sum % 10;   /* least significant digit */
        } else {
            a[++len] = sum;        /* single digit */
        }
        ++i;   /* move to the next adjacent pair */
    }

    seq_t result = a[n];
    free(a);
    return result;
}
