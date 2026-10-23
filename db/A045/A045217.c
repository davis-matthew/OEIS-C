/*
 * A045217  Numbers whose base-5 representation contains exactly three 0's and
 *          three 4's.
 * https://oeis.org/A045217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth smallest integer whose base‑5 representation
 *            contains exactly three 0's and three 4's
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* a(1) is the first admissible integer. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, but return 0 */

    idx_t found = 0;                      /* how many admissible numbers we've seen */
    uint64_t pow5 = 1;                    /* 5^L, as a 64‑bit integer */
    for (idx_t L = 1; ; ++L) {            /* iterate on the number of base‑5 digits */
        /* Multiply by 5 to obtain the next power */
        if (pow5 > UINT64_MAX / 5u) break; /* safety: avoid overflow; sequence gets very large beyond this point */
        pow5 *= 5ULL;
        idx_t start = pow5 / 5ULL;        /* 5^(L‑1) ; ensures leading digit ≠ 0 */
        idx_t end   = pow5 - 1ULL;        /* 5^L – 1 */
        for (idx_t x = start; x <= end; ++x) {
            /* Count 0's and 4's in the base‑5 representation of x */
            int cnt0 = 0, cnt4 = 0;
            seq_t v = (seq_t)x;           /* cast to seq_t to use seq_t arithmetic */
            while (v != 0) {
                int d = (int)(v % 5);
                if (d == 0) ++cnt0;
                else if (d == 4) ++cnt4;
                v /= 5;
            }
            if (cnt0 == 3 && cnt4 == 3) {
                ++found;
                if (found == n) return (seq_t)x;
            }
        }
    }
    /* If we exit the loop, the requested n is out of range for the implemented range,
       but per specification this should not happen. */
    return (seq_t)0;
}
