/*
 * A195770  Positive integer a is repeated m times, where m is the number of
 *          1-Pythagorean triples (a,b,c) satisfying a<=b.
 * https://oeis.org/A195770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1087, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th term of sequence A195770.
   For each integer a ≥ 1, let m(a) be the number of integer solutions
   (a,b,c) with a ≤ b ≤ c that satisfy c² = a² + b² + a·b
   (1‑Pythagorean triples).  Then the value a is repeated m(a) times.
   The routine generates terms on demand, stores them in a static
   array, and returns the n‑th term.  This is a search‑based construction,
   not an explicit closed formula. */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for 64‑bit values using binary search. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Count the number m(a) of 1‑Pythagorean triples with fixed first side a. */
static idx_t count_m(uint64_t a)
{
    uint64_t M = 3ULL * a * a;
    uint64_t sqrtM = isqrt_u64(M);
    idx_t cnt = 0;

    for (uint64_t s = 1; s <= sqrtM; ++s) {
        if (M % s != 0) continue;
        uint64_t t = M / s;          /* t ≥ s */

        /* (s + t) must be divisible by 4. */
        if (((s + t) & 3) != 0) continue;

        /* Parity requirement: s and a must have same parity so that b is integer. */
        if ((s & 1) != (a & 1)) continue;

        uint64_t c = (s + t) >> 2;   /* (s + t) / 4 */

        /* Compute b from d = 2c - s : b = (d - a)/2. */
        uint64_t d = (c << 1) - s;   /* d is even or odd depending on s */
        if (d < a) continue;         /* would give b < a */

        uint64_t b = (d - a) >> 1;   /* integer by construction */
        if (b >= a) cnt++;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state for memoised sequence generation */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of terms currently stored   */
    static size_t seq_cap = 0;   /* allocated capacity                  */
    static idx_t next_a = 0;     /* last processed integer a           */

    /* Return cached value if already computed */
    if (n <= seq_len) return seq[n - 1];

    /* Grow the array while we need more terms */
    while (seq_len < n) {
        ++next_a;                       /* next integer a to process      */
        idx_t m = count_m((uint64_t)next_a);      /* multiplicity for this a */

        /* Ensure enough capacity: double size as needed */
        if (seq_len + m > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 8;
            while (seq_len + m > new_cap) new_cap *= 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* Append this a, m times */
        for (idx_t i = 0; i < m; ++i)
            seq[seq_len++] = (seq_t)next_a;
    }

    return seq[n - 1];
}
