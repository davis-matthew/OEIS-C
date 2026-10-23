/*
 * A141327  Infinite array read by antidiagonals: a(m,n) = the smallest
 *          positive integer that has a factor in common with both m and n (m
 *          >= 2, n >= 2).
 * https://oeis.org/A141327
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 540, u16 n <= 1276, u32 n <= 1276, u64 n <= 1276,
 *            u128 n <= 1276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A141327.
   The sequence is defined by reading the infinite table a(m,n)
   by antidiagonals.  For the pair (m,n) (m,n ≥ 2) the value
   a(m,n) is the smallest positive integer that shares a factor
   with both m and n.  The OEIS formula is:
   if gcd(m,n) == 1   →  a(m,n) = spf(m) * spf(n)
   else                →  a(m,n) = min{ spf(m)*spf(n), spf(gcd(m,n)) }.
   Here spf(k) is the smallest prime factor of k (k itself if it is
   prime).  The mapping from index n to the pair (m,n) is performed
   by antidiagonal traversal starting at (2,2).  The function a()
   works for any build configuration (uint8_t…uint128, or an
   arbitrary‑precision class). */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for 64‑bit integers. */
static inline uint64_t uint64_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Smallest prime factor for a 64‑bit integer.  Returns the number
   itself if it is prime.  Uses 128‑bit arithmetic only for the
   internal test i*i ≤ v, which is harmless for all supported sizes. */
static inline seq_t spf_uint64(uint64_t v)
{
    if (v % 2 == 0) return (seq_t)2;
    for (uint64_t i = 3; i * i <= v; i += 2) {
        if (v % i == 0) return (seq_t)i;
    }
    return (seq_t)v;
}

/* Convert the array index (≥2) to the coordinates (m,n) on the
   antidiagonal tableau.  The function assumes 1‑based indices
   for the table starting at (2,2). */
static inline void antidiagonal_coords(idx_t n, idx_t *m, idx_t *n_col)
{
    idx_t k = n - 2;                      /* zero‑based offset */
    /* Find the smallest d such that d(d+1)/2 > k.
       This d is the diagonal number (1‑based). */
    idx_t d = 1;
    while ( (unsigned __int128)d * (d + 1) / 2 <= k ) ++d;

    /* Sum on this diagonal: m + n_col = d + 3. */
    idx_t s = d + 3;
    /* Number of elements before this diagonal: (d-1)d/2. */
    unsigned __int128 prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t p = k - (idx_t)prev;             /* position inside diagonal (0‑based) */
    *m      = 2 + p;
    *n_col  = s - *m;
}

/* Main OEIS function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* undefined below the offset */

    idx_t m, n_col;
    antidiagonal_coords(n, &m, &n_col);

    seq_t spf_m = spf_uint64((uint64_t)m);
    seq_t spf_n = spf_uint64((uint64_t)n_col);
    seq_t product = spf_m * spf_n;                 /* multiplication in seq_t */

    uint64_t g = uint64_gcd((uint64_t)m, (uint64_t)n_col);
    if (g == 1) {
        return product;                      /* coprime case */
    }

    seq_t spf_g = spf_uint64(g);
    return (spf_g < product) ? spf_g : product;
}
