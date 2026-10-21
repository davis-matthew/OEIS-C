/*
 * A235291  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235291
   a(1) = 40
   a(2) = 108
   a(3) = 272
   a(4) = 744
   For n ≥ 5: a(n) = 14*a(n-2) – 48*a(n-4)          */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset and the very first terms explicitly.  The sequence
       is 1‑based; a(0) is undefined, but we return 0 for safety. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)108;
    if (n == 3) return (seq_t)272;
    if (n == 4) return (seq_t)744;

    /* The recurrence depends on a(i‑2) and a(i‑4); a rolling buffer of
       the last four values suffices.  We initialise the buffer with
       a(1)…a(4).  For each subsequent i we compute a(i) from the
       buffer, then shift the buffer forward. */
    seq_t s0 = (seq_t)40;   /* a(i-4) */
    seq_t s1 = (seq_t)108;  /* a(i-3) */
    seq_t s2 = (seq_t)272;  /* a(i-2) */
    seq_t s3 = (seq_t)744;  /* a(i-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = s2 * (seq_t)14 - s0 * (seq_t)48;
        /* shift buffer */
        s0 = s1;
        s1 = s2;
        s2 = s3;
        s3 = new_val;
    }
    return s3;
}
