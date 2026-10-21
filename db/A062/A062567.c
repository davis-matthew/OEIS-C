/*
 * A062567  First multiple of n whose reverse is also divisible by n, or 0 if
 *          no such multiple exists.
 * https://oeis.org/A062567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062567
   a(n): first positive multiple of n whose decimal reversal is also
   divisible by n; 0 if no such multiple exists.
   The algorithm searches multiples of n until it satisfies the
   reversal divisibility test.  For n that contain both factors 2 and 5
   (i.e., n % 10 == 0) the answer is known to be 0, so the algorithm
   handles that case immediately.  For all other n a solution exists,
   and the brute‑force search converges quickly enough for n up to
   ~10⁴. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* reverse a positive integer; 0 is not used as a candidate */
static inline seq_t reverse_uint(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * (seq_t)10 + (x % (seq_t)10);
        x /= (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* memoization could be added if desired, but is not required */
    if (n == 0) return 0;          /* safety, though n>=1 by definition */
    if ((n & 1) == 0 && (n % 5) == 0) return 0;  /* n divisible by 10 → 0 */

    seq_t N = (seq_t)n;            /* cast once for arithmetic */
    seq_t m = 0;                   /* current multiple m = N * k  */
    while (1) {
        m += N;                    /* m = k * N  */

        /* check if reverse(m) is divisible by n */
        seq_t rev = reverse_uint(m);
        if (rev % (seq_t)n == 0) {
            return m;
        }
    }
}
