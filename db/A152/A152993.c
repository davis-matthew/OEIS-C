/*
 * A152993  a(n) = n - d(n) - pi(n) + 1.
 * https://oeis.org/A152993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - A000005(n) - A000720(n) + 1
 * Range:     u8 n <= 325, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime count (A000720) is obtained via compute_pi(n).
   Divisor count (A000005) is obtained via divisor_count_u64(n). */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                      /* undefined for n = 0, return 0 */
    seq_t n_val = (seq_t)n;                   /* convert index to seq_t */
    seq_t d_val = (seq_t)divisor_count_u64((uint64_t)n); /* d(n) */
    seq_t pi_val = compute_pi(n);              /* π(n) */
    return n_val - d_val - pi_val + (seq_t)1;  /* a(n) = n - d(n) - π(n) + 1 */
}
