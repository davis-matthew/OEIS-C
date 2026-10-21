/*
 * A067205  Least numbers with n = digital root = number of prime factors.
 * https://oeis.org/A067205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   count_pf_uint64, digit_root_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least numbers with n = digital root = number of prime factors.
   a(n) = the smallest positive integer m such that
   digital_root(m) = number_of_prime_factors(m) = n.
   For n > 9 no such integer exists, so a(n) = 0. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_pf_uint64.h"
#include "../utils/digit_root_uint64.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Memoisation table for the nine terms. */
    static bool init = false;
    static seq_t val[10] = {0};        /* indices 1..9 used, 0 unused */
    if (!init) {
        init = true;
        uint64_t m = 1;
        size_t found = 0;
        while (found < 9) {
            unsigned int dr = digit_root_uint64(m);
            if (dr != 0) {
                unsigned int pf = count_pf_uint64(m);
                if (pf == dr) {
                    unsigned int d = dr;
                    if (d <= 9 && val[d] == 0) {
                        val[d] = (seq_t)m;
                        ++found;
                    }
                }
            }
            ++m;
        }
    }

    if (n >= 1 && n <= 9)
        return val[(unsigned long long)n];
    return 0;          /* no number exists for n > 9 */
}
