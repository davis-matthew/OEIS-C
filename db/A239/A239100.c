/*
 * A239100  Solution to the problem of finding the number of comparisons
 *          needed for optimal merging of 3 elements with n elements.
 * https://oeis.org/A239100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3,
   a(6)=4, a(7)=6, a(8)=8,
   for n>8:
   a(3r)   = floor(43*2^(r-2)/7) - 2,
   a(3r+1) = floor(107*2^(r-3)/7) - 2,
   a(3r+2) = floor((17*2^r-6)/7) - 1.  (r ≥ 3)
*/
#include <stddef.h>

static const seq_t init_vals[9] = {0,0,1,1,2,3,4,6,8};

seq_t a(idx_t n)
{
    if (n <= 8) /* direct lookup for the first eight terms */
        return init_vals[n];

    idx_t r;
    size_t exp;
    seq_t pow2;

    switch (n % 3)
    {
    case 0:                     /* n = 3r   */
        r   = n / 3;
        exp = (size_t)(r - 2);  /* r ≥ 3 => r-2 ≥ 1 */
        pow2 = ((seq_t)1 << exp);
        return (43 * pow2) / 7 - 2;

    case 1:                     /* n = 3r+1 */
        r   = (n - 1) / 3;
        exp = (size_t)(r - 3);  /* r ≥ 3 => r-3 ≥ 0 */
        pow2 = ((seq_t)1 << exp);
        return (107 * pow2) / 7 - 2;

    case 2:                     /* n = 3r+2 */
        r   = (n - 2) / 3;
        exp = (size_t)r;       /* r ≥ 3 */
        pow2 = ((seq_t)1 << exp);
        return ((17 * pow2 - 6) / 7) - 1;
    }

    /* unreachable but required to satisfy the compiler */
    return (seq_t)0;
}
