/*
 * A009945  Coordination sequence for NiAs(2), As position.
 * https://oeis.org/A009945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(4n) = 2*((6n)^2 + 1); a(4n+2) = 18*(2n+1)^2; a(2n+1)
 *            = 6*(3n^2 + 3n + 1);
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* use n % 4 to decide the case */
    idx_t rem = n & 3ULL;  /* same as n % 4 for unsigned integers */

    if (rem == 0) { /* n = 4k */
        seq_t k = (seq_t)(n / 4);
        seq_t t   = (seq_t)6 * k;          /* t = 6k */
        return (seq_t)2 * (t * t + (seq_t)1);
    }

    if (rem == 2) { /* n = 4k + 2 */
        seq_t k = (seq_t)((n - 2) / 4);
        seq_t t   = (seq_t)2 * k + (seq_t)1;  /* t = 2k + 1 */
        return (seq_t)18 * (t * t);
    }

    /* odd indices: n = 2k + 1 (rem == 1 or 3) */
    seq_t k   = (seq_t)((n - 1) / 2);
    seq_t k2  = k * k;                        /* k^2 */
    seq_t t   = (seq_t)3 * k2 + (seq_t)3 * k + (seq_t)1; /* 3k^2 + 3k + 1 */
    return (seq_t)6 * t;
}
