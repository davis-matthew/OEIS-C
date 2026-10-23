/*
 * A194033  Inverse permutation of A194032; contains every positive integer
 *          exactly once.
 * https://oeis.org/A194033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(x + z, 2) + y + 1 where x = floor(sqrt(n)), y =
 *            n - x^2, and z = min(y, floor(y/2 + 1))
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned 64‑bit values */
static inline idx_t int_isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {            /* prevent division by zero when n = 0 */
            if (high == 0) break;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {     /* mid*mid <= n ? */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Expected offset is 1; for n == 0 simply return 0 (irrelevant) */
    if (n == 0) return 0;

    idx_t x = int_isqrt_u64(n);          /* floor(sqrt(n))               */
    idx_t y = n - x * x;                 /* remainder                     */
    seq_t Y = (seq_t)y;                  /* promote to seq_t              */

    seq_t Z1 = (Y >> 1) + 1;             /* floor(y/2) + 1                */
    seq_t Z  = (Y < Z1) ? Y : Z1;        /* min(y, floor(y/2)+1)          */

    seq_t X   = (seq_t)x;                /* promote x to seq_t            */
    seq_t sum = X + Z;                   /* x + z                         */
    seq_t bin = sum * (sum - 1) / 2;     /* binomial(sum,2)               */

    seq_t result = bin + Y + 1;          /* final formula                 */
    return result;
}
