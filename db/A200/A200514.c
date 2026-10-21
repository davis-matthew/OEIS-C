/*
 * A200514  Least m>0 such that n = y^2 - 4^x (mod m) has no solution, or 0 if
 *          no such m exists.
 * https://oeis.org/A200514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive m such that n = y^2 - 4^x (mod m) has no
 *            solution, 0 if every modulus allows a solution
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 688, u128 n <= 688,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;          /* use seq_t for all arithmetic */
    /* Step 1: check whether n is representable by y^2 - 4^x over integers. */
    seq_t pow4 = 1;
    for (idx_t x = 0; x < 25; ++x) { /* 4^25 > 1e15, more than enough for n <= 1e4 */
        seq_t tmp = n_seq + pow4;
        seq_t s = floor_sqrt(tmp);
        if (s * s == tmp) {          /* perfect square found → representable */
            return 0;
        }
        pow4 *= 4;
    }

    /* Step 2: search for the smallest m for which no solution exists. */
    for (uint32_t m = 1; ; ++m) {
        /* Precompute quadratic residues modulo m. */
        unsigned char *qr = (unsigned char *)malloc((size_t)m);
        unsigned char *seen_pow = (unsigned char *)malloc((size_t)m);
        if (!qr || !seen_pow) { /* allocation failure – fall back to 0 */
            free(qr);
            free(seen_pow);
            return 0;
        }
        memset(qr, 0, (size_t)m);
        memset(seen_pow, 0, (size_t)m);

        for (uint32_t y = 0; y < m; ++y) {
            uint32_t r = (uint32_t)((seq_t)y * (seq_t)y % (seq_t)m);
            qr[r] = 1;
        }

        /* Compute all distinct residues of 4^x (mod m) and test. */
        seq_t n_mod = n_seq % (seq_t)m;
        bool solvable = false;
        uint32_t val = (m == 0) ? 0 : 1 % m;           /* 4^0 = 1 */
        while (!seen_pow[val]) {
            seen_pow[val] = 1;
            uint32_t r = (uint32_t)((n_mod + (seq_t)val) % (seq_t)m);
            if (qr[r]) {                            /* found a solution modulo m */
                solvable = true;
                break;
            }
            val = (uint32_t)((val * 4) % m);
        }

        free(qr);
        free(seen_pow);

        if (!solvable) {           /* no solution for this m → minimal m found */
            return (seq_t)m;
        }
        /* Otherwise continue searching with next m. */
    }
}
