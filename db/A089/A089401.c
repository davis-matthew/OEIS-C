/*
 * A089401  a(n) = m - A089398(2^m + n) for m>=n.
 * https://oeis.org/A089401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A089401:  a(n) = m - A089398(2^m + n)  (m ≥ n, offset = 1)
   Recurrence used (from the OEIS entry):
      a(2^m)     = 2^m - m                     for all m ≥ 0
      a(2^m + k) = a(k) + 2^m + k - m         for 0 ≤ k ≤ m
      a(2^m + k) = a(k) + 2^m - 1             for m < k ≤ 2^m
   The implementation follows that recurrence iteratively, using the helper
   function floor_log2_uint64() to locate the highest power of two ≤ n. */

#include "../utils/floor_log2_uint64.h"

/* Computes A089401 for a given n (n ≥ 1).  Returns 0 for n == 0 to avoid
   undefined behaviour when called with an index below the offset. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety guard, offset starts at 1 */

    seq_t result = 0;

    /* Process n by repeatedly stripping off its highest power of two. */
    while (1) {
        idx_t m = floor_log2_uint64(n);   /* m = ⌊log₂ n⌋, 2^m ≤ n < 2^(m+1) */
        idx_t p = ((idx_t)1) << m;        /* p = 2^m */

        if (n == p) {                    /* n itself is a power of two */
            result += (seq_t)(n - m);
            break;
        }

        idx_t k = n - p;                  /* remainder after removing 2^m */

        if (k <= m) {
            /* Case: 2^m + k with k ≤ m  →  a(n) = a(k) + 2^m + k - m */
            result += (seq_t)(n - m);     /* 2^m + k - m = n - m */
        } else {
            /* Case: 2^m + k with k > m  →  a(n) = a(k) + 2^m - 1 */
            result += (seq_t)(p - 1);     /* 2^m - 1 */
        }

        /* Continue with the smaller argument k. */
        n = k;
    }

    return result;
}
