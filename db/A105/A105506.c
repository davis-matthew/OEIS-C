/*
 * A105506  Numbers m such that 6 is the leading digit of the n-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A105506: a(n) = the n-th index m such that the n-th Fibonacci
 * number F_m starts with the digit 6.  The implementation uses the
 * Benford-law approximation:  leading digit of F_k is determined by
 * the fractional part of k·α – β, where α = log10(ϕ) and β = log10(√5).
 * Those constants are pre‑scaled to 10¹² and compared with the
 * integer thresholds for leading digit 6:
 *
 *   L = 0.778151250000 × 10¹²  (lower bound for digit 6)
 *   U = 0.845098040000 × 10¹²  (upper bound for digit 6)
 *
 * The routine scans Fibonacci indices sequentially, caching results
 * so that calls with arbitrary n are efficient.  Complexity is O(a(n))
 * ≤ O(2.0·10⁶) for n ≤ 10 000, comfortably within the limits. */

#include <stdlib.h>
#include <stdint.h>

/* Scaled constants (fixed‑point with 12 decimal digits) */
static const unsigned long long SCALE = 1000000000000ULL;
static const unsigned long long A     = 208987640250ULL;   /* α·SCALE          */
static const unsigned long long B     = 349485002168ULL;   /* β·SCALE          */
static const unsigned long long L     = 778151250000ULL;   /* log10(6)·SCALE   */
static const unsigned long long U     = 845098040000ULL;   /* log10(7)·SCALE   */

seq_t a(idx_t n)
{
    /* Lazy static allocations and caching */
    static seq_t *vals = NULL;
    static idx_t  cap   = 0;
    static idx_t  size  = 0;
    static idx_t  next  = 1;   /* next Fibonacci index to test */

    if (n == 0)
        return 0;  /* out of range, not used in the OEIS offset */

    /* Ensure we have computed up to the requested term */
    while (size < n) {
        /* Compute fractional part f = (next*A - B) mod SCALE */
        __int128 prod = (__int128)next * (__int128)A - (__int128)B;
        __int128 rem  = prod % (__int128)SCALE;
        if (rem < 0)
            rem += (__int128)SCALE;
        unsigned long long f = (unsigned long long)rem;

        /* Check if leading digit is 6 */
        if (f >= L && f < U) {
            /* Grow array if necessary */
            if (size >= cap) {
                idx_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *newptr = (seq_t *)realloc(vals, newcap * sizeof(seq_t));
                if (!newptr) abort();  /* allocation failed */
                vals = newptr;
                cap  = newcap;
            }
            vals[size++] = (seq_t)next;  /* store 6‑leading index */
        }
        ++next;
    }
    return vals[n - 1];
}
