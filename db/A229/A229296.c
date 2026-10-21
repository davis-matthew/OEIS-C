/*
 * A229296  Number of solutions to x^2 + y^2 == n (mod 2*n) for x,y in [0,
 *          2*n).
 * https://oeis.org/A229296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A086933(n) – the number of solutions to x^2 + y^2 ≡ n
 *            (mod 2n) with 0 ≤ x,y < 2n. The function works for all supported
 *            seq_t and idx_t types
 * Range:     u8 n <= 8, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Maximum possible 2n for n up to 10000 -> m ≤ 20000.
   Using a static array avoids dynamic allocation and
   guarantees proper construction for any seq_t type. */
#define MAX_M   40001
static seq_t cnt[MAX_M];

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n = 0 return 0 gracefully. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Modulus m = 2 * n.  n fits in idx_t. */
    idx_t m = (idx_t)2 * n;

    /* Ensure the indexing range is within the static buffer. */
    if (m > (idx_t)(MAX_M - 1))
        return (seq_t)0;   /* should not happen for the intended limits */

    /* Clear the counter array up to m. */
    for (idx_t i = (idx_t)0; i < m; ++i)
        cnt[i] = (seq_t)0;

    /* Count how many y give each residue y^2 (mod m). */
    for (idx_t y = (idx_t)0; y < m; ++y) {
        seq_t ysq = (seq_t)y * (seq_t)y;
        seq_t res = ysq % (seq_t)m;          /* 0 ≤ res < m */
        cnt[(size_t)res] += (seq_t)1;
    }

    /* For each x, add the number of y that satisfy the congruence. */
    seq_t total = (seq_t)0;
    for (idx_t x = (idx_t)0; x < m; ++x) {
        seq_t xsq = (seq_t)x * (seq_t)x;
        seq_t xmod = xsq % (seq_t)m;          /* x^2 (mod m) */
        /* The required residue for y^2 is n - x^2 (mod m). */
        seq_t req = ((seq_t)n - xmod + (seq_t)m) % (seq_t)m;
        total += cnt[(size_t)req];
    }

    return total;
}
