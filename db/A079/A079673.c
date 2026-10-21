/*
 * A079673  Consider pairs (r,s) such that the polynomial (x^r+1) divides
 *          (x^s+1) and 1 <= r < s. This sequence gives the r values; A079581
 *          gives the s values.
 * https://oeis.org/A079673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   sort_uchar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates the pairs (r,s) with 1 ≤ r < s and s/r an odd integer.
   The pairs are sorted by increasing s, and for a fixed s by increasing r.
   The function returns r at position n (1‑based). */

#include <stddef.h>
#include "../utils/sort_uchar.h"

seq_t a(idx_t n)
{
    idx_t target = n;          /* 1‑based target index */
    idx_t count  = 0;          /* how many pairs have been seen */

    for (idx_t s = 3; ; ++s) { /* start from the smallest s that can have r< s */
        /* Split s into m * 2^k, where m is odd */
        idx_t tmp = s;
        idx_t k   = 0;
        while ((tmp & 1ULL) == 0) {
            ++k;
            tmp >>= 1;
        }
        idx_t m = tmp;

        if (m == 1)               /* s is a power of two -> no valid r */
            continue;

        /* Base factor 2^k */
        idx_t base = s / m;       /* equals 1ULL << k */

        /* Gather divisors of the odd part m that are strictly less than m */
        unsigned char divs[64];
        size_t divcnt = 0;

        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                if (d < m && divcnt < sizeof(divs)) divs[divcnt++] = (unsigned char)d;
                idx_t e = m / d;
                if (e != d && e < m && divcnt < sizeof(divs)) divs[divcnt++] = (unsigned char)e;
            }
        }

        if (divcnt == 0) /* m has no proper divisors (i.e., m is 1, handled earlier) */
            continue;

        sort_uchar(divs, divcnt);

        /* Enumerate the r values for this s, in ascending order */
        for (size_t i = 0; i < divcnt; ++i) {
            seq_t r = (seq_t)(base * divs[i]); /* r = 2^k * d */
            ++count;
            if (count == target)
                return r;
        }
    }
}
