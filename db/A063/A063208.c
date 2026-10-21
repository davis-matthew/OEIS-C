/*
 * A063208  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          36 ).
 * https://oeis.org/A063208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063208
   Formula: a(n) = a(r) + 5 * floor((n-1)/6)
   where r = ((n-1) mod 6) + 1 and
   a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=4.
   This follows from the recurrence a(n+6)=a(n)+5. */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* pre‑computed values for 1 ≤ n ≤ 6 */
    static const seq_t init[7] = {
        0, /* unused, index 0 */
        1, /* a(1) */
        1, /* a(2) */
        2, /* a(3) */
        3, /* a(4) */
        4, /* a(5) */
        4  /* a(6) */
    };

    /* find the index r within the first 6 terms */
    idx_t r = ((n - 1) % 6) + 1;      /* 1 … 6 */
    /* number of full periods of length 6 preceding n */
    idx_t m = (n - 1) / 6;            /* floor((n-1)/6) */

    /* compute a(n) = init[r] + 5*m  */
    /* cast m to seq_t before multiplying to stay in proper type */
    seq_t part = (seq_t)5 * (seq_t)m;
    return init[r] + part;
}
