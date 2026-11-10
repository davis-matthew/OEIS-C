/*
 * A227745  Nonnegative integers whose squares occur in A173318.
 * https://oeis.org/A227745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(A227744(n)), where A227744 lists the squares
 *            occurring in A173318(k) = Sum_{i=0..k} A005811(i), A005811(i) =
 *            popcount(i XOR (i>>1)); computed by a running scan
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 937, u64 n <= 937,
 *            u128 n <= 937, big any n
 * Validated: standard, pass, 76 terms through n = 876 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t count = 0, cap = 0;
    static idx_t i = 0;          /* current index into A173318 */
    static seq_t s = 0;          /* A173318(i) */
    static seq_t m = 0;          /* candidate root */
    static seq_t sq = 0;         /* m*m */

    if (n < 1) return (seq_t)0;
    while (count < n) {
        while (sq < s) {
            m++;
            sq = m * m;
        }
        if (sq == s) {
            if (count >= cap) {
                idx_t ncap = cap ? cap * 2 : 1024;
                seq_t *nt = mem_resize(seq_t, terms, (size_t)ncap);
                if (nt == NULL) return (seq_t)0;
                terms = nt;
                cap = ncap;
            }
            terms[count++] = m;
            m++;
            sq = m * m;
        }
        i++;
        uint64_t x = (uint64_t)i;
        s += (seq_t)__builtin_popcountll(x ^ (x >> 1));
    }
    return terms[n - 1];
}
