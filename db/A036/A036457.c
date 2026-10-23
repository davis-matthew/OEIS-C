/*
 * A036457  Numbers k for which exactly 5 applications of A000005 are needed
 *          to reach 2.
 * https://oeis.org/A036457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036457: numbers k for which exactly 5 applications of  
   the divisor counting function (A000005) are needed to reach 2.  
   Calculation is performed by brute‑force search. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

/* Return true iff exactly five iterations of the divisor count function
   produce the value 2, with no earlier iteration reaching 2. */
static inline bool is_depth5(uint64_t n)
{
    uint64_t cur = n;
    for (int d = 1; d <= 5; ++d)
    {
        cur = divisor_count_u64(cur);
        if (cur == 2)
            return d == 5;          /* true only if reached at depth 5 */
    }
    return false;                   /* never reached 2 in five steps */
}

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for non‑positive indices */

    uint64_t need = (uint64_t)n;   /* target count in uint64_t for speed */
    uint64_t found = 0;
    uint64_t k = 1;                /* current integer to test */

    while (true)
    {
        if (is_depth5(k))
        {
            ++found;
            if (found == need)
                return (seq_t)k;   /* cast to seq_t (construction or conversion) */
        }
        ++k;
    }
}
