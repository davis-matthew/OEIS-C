/*
 * A063656  Numbers k such that the truncated square root of k is equal to the
 *          rounded square root of k.
 * https://oeis.org/A063656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063656
   Numbers k such that the truncated (floor) square root of k
   is equal to the rounded square root of k.
   Formula:
     Let k be the largest integer with k(k+1)/2 ≤ n.
     Then a(n) = k² + (n - k(k+1)/2)
     where k = ⌊(√(8n + 1) – 1)/2⌋.
   The computation below is O(1) and works for any idx_t n that
   fits in an unsigned 64‑bit integer.  All intermediate values
   are kept within seq_t by casting appropriately.  */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static inline idx_t isqrt_u64(uint64_t x)
{
    if (x < 2) return (idx_t)x;
    long double d = sqrtl((long double)x);
    uint64_t r = (uint64_t)d;
    /* adjust for rounding errors */
    while ((r + 1ULL) * (r + 1ULL) <= x) r++;
    while (r * r > x) r--;
    return (idx_t)r;
}

seq_t a(idx_t n)
{
    /* Step 1: find k = ⌊(√(8n + 1) – 1)/2⌋ */
    uint64_t val = n;
    uint64_t tmp = 8ULL * val + 1ULL;           /* 8n+1 fits in 64‑bit here */
    idx_t sqr = isqrt_u64(tmp);                 /* floor(sqrt(8n+1)) */
    idx_t k   = (sqr - 1) / 2;                   /* integer division */

    /* Step 2: compute the offset within the block for this k */
    seq_t m     = (seq_t)val;
    seq_t prev  = (seq_t)k * (k + 1ULL) / 2;     /* k(k+1)/2   */
    seq_t offset= m - prev;

    /* Step 3: compute the sequence value */
    seq_t result = (seq_t)k * (seq_t)k + offset;
    return result;
}
