/*
 * A194465  Numbers m such that Sum_{k=1..m} (<c + k*r> - <k*r>) < 0, where
 *          r=sqrt(2) and c=sqrt(1/2), and < > denotes fractional part.
 * https://oeis.org/A194465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that S(m) = Σ_{k=1..m} ( ⟨c + k·r⟩ -
 *            ⟨k·r⟩ ) < 0, where r = sqrt(2) and c = sqrt(1/2)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root for uint64_t via binary search. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * high <= x) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* The sequence cache.  We pre‑allocate enough room for the expected
   number of terms (≈ 200 000 for n up to ~10 000).  This is a static
   array so we avoid any dynamic reallocation that might not be
   compatible with arbitrary‑precision seq_t. */
#define MAX_TERMS 200000
static seq_t terms[MAX_TERMS];
static size_t term_count = 0;    /* number of terms stored */

/* current_m: last m processed for the cumulative computation.
   N2: number of k ≤ current_m with the fractional part of k*sqrt(2)
       ≥ 1 - sqrt(2)/2. */
static idx_t current_m = 0;
static idx_t N2 = 0;

/* Return the n‑th term, n ≥ 1.  The function is thread‑unsafe but
   satisfies the required interface. */
seq_t a(idx_t n)
{
    /* Compute new terms until we have at least n of them. */
    while (term_count < n) {
        current_m++;  /* new m to examine */

        /* Integer part of k*sqrt(2) via floor(sqrt(2*k*k)). */
        idx_t kk = current_m * current_m;
        uint64_t sq = (uint64_t)kk * 2ULL;               /* 2*k^2 */
        idx_t L = isqrt_u64(sq);                         /* floor(sqrt(2*k^2)) */

        /* Determine whether {k*sqrt(2)} ≥ 1 – sqrt(2)/2.
           Inequality: 4k^2 + 4k + 1 ≥ 2 (L + 1)^2.          */
        unsigned __int128 left  = 4ULL * (__uint128_t)kk + 4ULL * current_m + 1ULL;
        idx_t        Lp1    = L + 1;
        unsigned __int128 right = 2ULL * (__uint128_t)Lp1 * Lp1;

        if (left >= right)
            N2++;    /* this k contributes to the N2 count */

        /* Check whether S(current_m) < 0:
           condition becomes 2*N2^2 > current_m^2. */
        unsigned __int128 lhs = 2ULL * (__uint128_t)N2 * N2;
        unsigned __int128 rhs = (__uint128_t)current_m * current_m;

        if (lhs > rhs) {
            if (term_count >= MAX_TERMS) abort();   /* safety guard */
            terms[term_count++] = (seq_t)current_m;
        }
    }
    return terms[n - 1];
}
