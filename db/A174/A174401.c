/*
 * A174401  Sequence showing kinds of "waves", built as follows in comments.
 * https://oeis.org/A174401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174401
   a(0) = 1, a(1) = 6, a(2) = 26, a(3) = 2.
   For n ≥ 4:
      * if n = 2^k            → a(n) = (74*4^(k-1)+4)/3
      * if n = 3*2^(k-1)      → a(n) = (8*4^(k-2)+4)/3
      * otherwise put n into the lower block:
          let k = ⌊log₂ n⌋, p = 2^k, offset = n-p.
          if offset < p/2   → a(n) = a(p/2 + offset)
          if offset > p/2   → a(n) = a(p/2 + (offset‑p/2))
          (offset = p/2 is the middle element handled above)
   This is a simple O(log n) recursive implementation. */

#include <stdint.h>
#include <stdbool.h>

/* helper: power of two up to 64 bits */
static inline idx_t power_of_two(idx_t k) { return (idx_t)1 << k; }

/* helper: compute 4^e */
static inline seq_t pow4(unsigned e)
{
    seq_t v = 1;
    for (unsigned i = 0; i < e; ++i) v *= (seq_t)4;
    return v;
}

/* helper: floor log2 for 64‑bit unsigned integers */
static unsigned log2_floor(idx_t n)
{
    unsigned k = 0;
    while (n > 1) { n >>= 1; ++k; }
    return k;
}

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)26;
    if (n == 3) return (seq_t)2;

    /* handle powers of two */
    if ((n & (n - 1)) == 0) {          /* n is a power of two */
        unsigned k = log2_floor(n);    /* k ≥ 1 here */
        seq_t v = 74 * pow4(k - 1) + 4;
        return v / 3;
    }

    /* generic case: n is not a power of two, n ≥ 4 */
    unsigned k = log2_floor(n);        /* 2^k ≤ n < 2^(k+1) */
    idx_t     p    = power_of_two(k);  /* p = 2^k   */
    idx_t     off  = n - p;            /* 1 … 2^k-1 */
    idx_t     half = p >> 1;           /* 2^(k-1)  */

    if (off == half) {                 /* middle element of block */
        seq_t v = 8 * pow4(k - 2) + 4;
        return v / 3;
    } else if (off < half) {            /* left half → recurse */
        return a(half + off);
    } else {                            /* right half → recurse */
        return a(half + (off - half));
    }
}
