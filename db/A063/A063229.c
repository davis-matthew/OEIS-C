/*
 * A063229  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          69 ).
 * https://oeis.org/A063229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(k) + 44*q where k = n - 6 * ((n-2)/6), 1 <= k <= 7 and
 *            q = (n-2)/6. The first 7 terms are fixed: a(1)=3, a(2)=12,
 *            a(3)=18, a(4)=26, a(5)=32, a(6)=42, a(7)=46. For n>7 the
 *            recurrence a(n) = a(n-6) + 44 holds. The implementation uses
 *            only integer arithmetic and works for all seq_t and idx_...
 * Range:     u8 n <= 35, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[7] = {3u, 12u, 18u, 26u, 32u, 42u, 46u};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n < 1 */

    if (n <= 7)
        return base[n - 1];    /* return one of the stored first 7 terms */

    idx_t q = (n - 2) / 6;     /* how many full 6‑step jumps before n */
    idx_t k = n - 6 * q;       /* index in [1..7] after those jumps */

    return base[k - 1] + (seq_t)44 * q;
}
