/*
 * A136410  Numbers k having a proper divisor d > 2 such that d-1 divides k-1.
 * https://oeis.org/A136410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k > 0 such that there exists a proper
 *            divisor d>2 with (d‑1) | (k‑1)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if k meets the definition of the sequence. */
static bool has_condition(uint64_t k)
{
    if (k <= 8) return false;            /* the sequence starts at 9 */
    uint64_t km1 = k - 1;

    /* Check divisors d from 3 up to sqrt(k). */
    for (uint64_t d = 3; d * d <= k; ++d) {
        if (k % d == 0) {
            if (km1 % (d - 1) == 0) return true;

            uint64_t e = k / d;          /* paired divisor */
            if (e > 2 && e != k && km1 % (e - 1) == 0) return true;
        }
    }
    return false;
}

/* --------------------------------------------------------------------
   Return the n‑th term of the sequence (a(1)=9, a(2)=15, …).
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, safe fallback */

    idx_t found = 0;
    for (uint64_t k = 9; ; ++k) {
        if (has_condition(k)) {
            ++found;
            if ((idx_t)found == n) {
                return (seq_t)k;
            }
        }
    }
}
