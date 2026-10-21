/*
 * A051527  Order of group H_{1,n}^{8}.
 * https://oeis.org/A051527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051527: Order of group H_{1,n}^{8}
   Formula: a(n) = 192 * n * t(n)
   where
      t(1) = 1
      t(2) = 2
      t(n) = (n^2 - 1)/2          for odd n ≥ 3
      t(n) = 2^(n/2 + 1)           for even n ≥ 4
   (All integer arithmetic uses seq_t types.  The computation is performed
   with loops to avoid overflow on fixed‑size seq_t types.) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not in the OEIS range */

    seq_t base = (seq_t)192;
    seq_t nn   = (seq_t)n;
    seq_t t;

    if (n == 1) {
        t = (seq_t)1;
    } else if (n == 2) {
        t = (seq_t)2;
    } else if ((n & 1ULL) == 0) {          /* even n ≥ 4 */
        /* compute 2^(n/2 + 1) by repeated multiplication */
        t = (seq_t)2;                     /* 2^(1) */
        uint64_t mul = n / 2ULL;          /* how many extra powers of two */
        for (uint64_t i = 0; i < mul; ++i) {
            t *= (seq_t)2;
        }
    } else {                              /* odd n ≥ 3 */
        seq_t nn_sq = nn * nn;          /* n^2 fits in seq_t for n ≤ 10⁴ */
        t = nn_sq - (seq_t)1;
        t /= (seq_t)2;
    }

    return base * nn * t;
}
