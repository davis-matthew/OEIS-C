/*
 * A101646  Array read by antidiagonals: T(n,k) = variant of Knuth's Fibonacci
 *          (or circle) product of n and k (A101330). Sometimes called the
 *          "arroba" product.
 * https://oeis.org/A101646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) with T(i,j) = i*j –
 *            floor((j+1)/φ²)*floor((i+1)/φ²). Indices (i,j) are obtained by
 *            reading the triangle of all T(n,k) by antidiagonals (offset 1).
 *            Uses exact integer arithmetic; no floating point is used
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 int128_t;
#else
typedef unsigned long long int128_t;
#endif

/* Fibonacci constants for the rational approximation φ ≈ F46/F45.
   These constants are of type unsigned long long to avoid overflow in seq_t. */
static const unsigned long long FIB45 = 1134903170ULL;
static const unsigned long long FIB46 = 1836311903ULL;

/* floor(φ * n) */
static inline seq_t floor_phi(idx_t n)
{
    int128_t prod = (int128_t)n * (int128_t)FIB46;
    return (seq_t)(prod / (int128_t)FIB45);
}

/* floor((n+1)/φ²) using the identity
   ⌊(n+1)/φ²⌋ = 2n+1 – ⌊(n+1)φ⌋. */
static inline seq_t floor_phi2(idx_t n)
{
    seq_t fl = floor_phi(n + 1);
    int128_t tmp = (int128_t)2 * (int128_t)n + 1 - (int128_t)fl;
    return (seq_t)tmp;
}

/* Arroba product: T(n,k) = n*k – floor((k+1)/φ²)*floor((n+1)/φ²) */
static inline seq_t T(idx_t n, idx_t k)
{
    seq_t fn = floor_phi2(n);
    seq_t fk = floor_phi2(k);
    int128_t prod1 = (int128_t)n * (int128_t)k;
    int128_t prod2 = (int128_t)fn * (int128_t)fk;
    return (seq_t)(prod1 - prod2);
}

/* a(n): entry of the array read by antidiagonals of all T(n,k). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; undefined for n=0 */

    /* Find diagonal d such that (d-1)d/2 < n ≤ d(d+1)/2. */
    idx_t d = 1;
    idx_t cumulative = 1;              /* cumulative = d*(d+1)/2 after loop */
    while (n > cumulative) {
        ++d;
        cumulative += d;
    }

    /* Start index of diagonal d (0‑based). */
    idx_t start = cumulative - d;      /* (d-1)d/2 */
    idx_t offset = n - start - 1;      /* 0‑based offset within the diagonal */

    idx_t i = d - offset;              /* row index  (1‑based) */
    idx_t j = offset + 1;              /* column index (1‑based) */

    return T(i, j);
}
