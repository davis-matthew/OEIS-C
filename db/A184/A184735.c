/*
 * A184735  a(n)=floor(n*s+h-h*s), where s=(3+sqrt(5))/2, h=1/3; complement of
 *          A184734.
 * https://oeis.org/A184735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*s + h - h*s) with s = (3 + sqrt(5)) / 2 and h =
 *            1/3 Simplified: a(n) = n + floor( ( (3n-1) + sqrt(5)*(3n-1) ) /
 *            6 )
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for arbitrary-width integer type seq_t. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        /* Avoid mid*mid overflow by comparing mid with x/mid. */
        if (mid > x / mid) {          /* mid*mid > x */
            high = mid - 1;
        } else {                       /* mid*mid <= x */
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used for offset 1, but safe guard */

    /* Compute B = 3*n - 1 */
    seq_t B = ((seq_t)n * 3) - 1;

    /* Compute sqrt_term = floor_sqrt(5 * B * B) */
    seq_t BB = B * B;
    seq_t sb = BB * 5;
    seq_t sqrt_term = floor_sqrt(sb);

    /* G(B) = floor((B + sqrt_term) / 6) */
    seq_t g = (B + sqrt_term) / 6;

    /* a(n) = n + G(B) */
    return (seq_t)n + g;
}
