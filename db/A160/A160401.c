/*
 * A160401  Table read by antidiagonals: a(m,n) = the smallest composite
 *          multiple of both m and n.
 * https://oeis.org/A160401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the 2‑D table a(m,n) taken along
 *            antidiagonals. For each pair (m,n): * if lcm(m,n)=1 -> a(m,n)=4
 *            * if lcm(m,n) is prime -> a(m,n)=2*lcm(m,n) * otherwise ->
 *            a(m,n)=lcm(m,n) The diagonal indexing follows the OEIS
 *            convention: (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),... This
 *            function co...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t gcd64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Simple primality test for 64‑bit unsigned integers.  It is adequate
   for the small values encountered in this sequence (lcm(m,n) ≤ 2·10⁴
   for n ≤ 10 000). */
static inline bool is_prime64(idx_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    for (idx_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; 0 is unused */

    /* Find the diagonal d such that (d-1)*d/2 >= n. */
    idx_t d = 2;
    while (((d - 1) * d) / 2 < n) d++;

    /* Number of terms before diagonal d. */
    idx_t terms_before = ((d - 2) * (d - 1)) / 2;

    /* Position of the term on this diagonal (1‑based). */
    idx_t pos = n - terms_before;

    /* Coordinates (m,n) for this term. */
    idx_t m = pos;
    idx_t k = d - pos;   /* j = d - pos */

    /* Compute lcm(m,k). */
    idx_t g = gcd64(m, k);
    idx_t l = (m / g) * k;   /* m/g and k fit in 64 bits */

    if (l == 1) {
        return (seq_t)4;               /* a(1,1) = 4 */
    }

    if (is_prime64(l)) {
        return (seq_t)(2 * l);         /* 2·l for prime lcm */
    }

    return (seq_t)l;                   /* composite lcm */
}
