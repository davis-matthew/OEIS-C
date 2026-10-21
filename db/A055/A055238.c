/*
 * A055238  Numbers that are divisible by at least one of their digits in
 *          every base.
 * https://oeis.org/A055238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nice, base, nonn
 * Formula:   brute‑force search for the n‑th positive integer that is
 *            divisible by at least one of its base‑b digits for every base
 *            b≥2 (OEIS A055238)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   has_dividing_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_dividing_digit.h"

/* Return true if n meets the OEIS condition for all bases 2 … n. */
static bool is_good(uint64_t n)
{
    /* n=1 is always good: the loop body is skipped. */
    for (uint64_t b = 2; b <= n; ++b) {
        if (!has_dividing_digit(n, b)) {
            return false;
        }
    }
    return true;
}

/* Main routine: return the n-th term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; unused but harmless */

    uint64_t found = 0;        /* how many good numbers seen so far */
    uint64_t cand   = 0;        /* current candidate to test */

    while (found < n) {
        ++cand;
        if (is_good(cand)) {
            ++found;
        }
    }

    /* The OEIS entry shows the numbers are small (≤120 for the first
       24 terms); casting from uint64_t to seq_t is safe for all
       supported builds. */
    return (seq_t) cand;
}
