/*
 * A053592  Open 3-dimensional ball numbers (version 1): a(n) is the number of
 *          integer points (i,j,k) contained in an open ball of diameter n,
 *          centered at (0,0,0).
 * https://oeis.org/A053592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{(i,j,k) ∈ Z³ : i² + j² + k² < (n/2)²}|
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count integer lattice points in the open ball of diameter n.
   The implementation uses only integer arithmetic and an
   efficient integer square‑root routine.  For each n the loops
   run over i and j in the first octant (i , j ≥ 0) and add all
   sign combinations in a single multiplication.  The number of
   iterations is about π·(n/2)², which is <8·10⁷ for n ≤ 10 000
   and runs well within the time limits. */

#include <stddef.h>

static inline seq_t isqrt_seq(seq_t v)
{
    seq_t r = 0;
    /* largest power of four <= max of seq_t */
    seq_t b = ((seq_t)1) << ((sizeof(seq_t) * 8 - 2));
    while (b > v) b >>= 2;
    while (b != 0) {
        if (v >= r + b) {
            v -= r + b;
            r = (r >> 1) + b;
        } else {
            r >>= 1;
        }
        b >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* 4 * (i² + j² + k²) < n²  ->  i² + j² + k² <= (n²-1)/4 */
    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t limit = (n2 - 1) / 4;          /* upper bound for i²+j²+k² */

    seq_t max_i = isqrt_seq(limit);      /* floor(sqrt(limit)) */

    seq_t total = 0;
    for (seq_t i = 0; i <= max_i; ++i) {
        seq_t i2 = i * i;
        seq_t rem_i = limit - i2;
        seq_t max_j = isqrt_seq(rem_i);

        seq_t mult_i = (i == 0) ? 1 : 2;   /* +/− i, except i=0 */

        for (seq_t j = 0; j <= max_j; ++j) {
            seq_t j2 = j * j;
            seq_t rem_j = rem_i - j2;
            seq_t kmax = isqrt_seq(rem_j);

            seq_t mult_j = (j == 0) ? 1 : 2;   /* +/− j, except j=0 */
            seq_t mult = mult_i * mult_j;      /* sign choices for i and j */

            seq_t add = mult * (2 * kmax + 1); /* all k ∈ [−kmax, kmax] */
            total += add;
        }
    }
    return total;
}
