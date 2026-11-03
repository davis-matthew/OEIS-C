/*
 * A140485  Trajectory of 1 under repeated application of the map: n -> n +
 *          second-smallest number that does not divide n.
 * https://oeis.org/A140485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8265 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper: return the second smallest positive integer that does not divide x */
static inline seq_t second_non_divisor(seq_t x)
{
    idx_t missing = 0;          /* count of non‑divisors found */
    for (idx_t k = 2; ; ++k)   /* k starts at 2 because 1 always divides x */
    {
        if (x % (seq_t)k != 0)  /* k does not divide x */
        {
            ++missing;
            if (missing == 2)   /* second non‑divisor found */
                return (seq_t)k;
        }
    }
}
/* A140485: a(1)=1.
   a(n)=a(n-1)+b, where b is the second‑smallest positive integer that does not divide a(n-1). */

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for 0, fallback */

    seq_t cur = 1;             /* a(1) */
    if (n == 1) return cur;

    /* iterate from 2 up to n, computing each term on the fly */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t inc = second_non_divisor(cur);
        cur += inc;            /* a(i) = a(i-1)+inc */
    }
    return cur;
}
