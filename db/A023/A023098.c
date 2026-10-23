/*
 * A023098  a(n) is least k such that k and 6k are anagrams in base n (written
 *          in base 10).
 * https://oeis.org/A023098
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023098: a(n) = least k such that k and 6k are anagrams in base n
   (digits are written in base 10).  The first term corresponds to n = 7. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 7) {
        return (seq_t)0;      /* undefined for n < 7 */
    }

    /* k must be a multiple of L = (n-1)/gcd(5,n-1). */
    idx_t nm1 = n - 1;
    idx_t g = 1;               /* gcd(5, nm1) is either 1 or 5 */
    if (nm1 % 5 == 0) {
        g = 5;
    }
    idx_t L = nm1 / g;

    /* Allocate array for digit frequencies. */
    unsigned *cnt = (unsigned *)calloc((size_t)n, sizeof(unsigned));
    if (!cnt) {                /* allocation failure – return 0 */
        return (seq_t)0;
    }

    const seq_t six = (seq_t)6;

    /* Search over multiples of L. */
    idx_t m = 1;
    for (;;) {
        seq_t k = (seq_t)m * L;

        /* Reset digit counts. */
        size_t sz = (size_t)n;
        for (size_t i = 0; i < sz; ++i) {
            cnt[i] = 0;
        }

        /* Count digits of k. */
        seq_t v = k;
        if (v == 0) {
            cnt[0] = 1;
        } else {
            while (v > 0) {
                size_t d = (size_t)(v % (seq_t)n);
                ++cnt[d];
                v /= (seq_t)n;
            }
        }

        /* Subtract digits of 6*k. */
        v = k * six;
        while (v > 0) {
            size_t d = (size_t)(v % (seq_t)n);
            --cnt[d];
            v /= (seq_t)n;
        }

        /* Check if all counts are zero. */
        bool same = true;
        for (size_t i = 0; i < sz; ++i) {
            if (cnt[i] != 0) {
                same = false;
                break;
            }
        }
        if (same) {
            free(cnt);
            return k;
        }

        ++m;
    }
}
