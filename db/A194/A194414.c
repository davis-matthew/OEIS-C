/*
 * A194414  Numbers m such that Sum_{k=1..m} (<1/3 + k*r> - <k*r>) > 0, where
 *          r=sqrt(2) and < > denotes fractional part.
 * https://oeis.org/A194414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A194414: Numbers m such that 
 *     Sum_{k=1..m} ( {1/3 + k*√2} – {k*√2} ) > 0
 * where {x} denotes the fractional part of x.
 * 
 * The term inside the sum is +1/3 if {k*√2} < 2/3 and –2/3 otherwise.
 * Let A_m be the number of k ≤ m with {k*√2} < 2/3. Then
 *      3 · S_m = 3 A_m – 2 m
 * and S_m > 0 ⇔ 3 A_m > 2 m.
 * 
 * For each integer k we can decide whether {k*√2} < 2/3 using only
 * integer arithmetic as follows:
 *   a = floor(√(2 k²))   // a = ⌊k√2⌋
 *   δ = 1  iff   18 k²  <  (3 a + 2)²
 * The condition follows from
 *      {k√2} < 2/3  ⇔  3k√2 < 3a + 2  ⇔  18k² < (3a+2)²
 * 
 * The sequence is generated lazily:
 *   * Keep a running count A of the k’s that satisfy the inequality.
 *   * Whenever 3*A > 2*m, store m as the next term.
 *   * Subsequent calls arbitrarily out‑of‑order are supported via
 *     memoisation of the list of found terms.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit value (floor) using Newton's method. */
static uint64_t isqrt_u64(uint64_t n) {
    if (n < 2) return n;
    uint64_t x0 = n, x1 = (x0 + 1) >> 1;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + n / x0) >> 1;
    }
    return x0;
}

/* Return the n-th term of A194414. */
seq_t a(idx_t n) {
    static seq_t *cache   = NULL;   /* stored terms (m values)            */
    static size_t cache_cap = 0;     /* capacity of cache array             */
    static size_t cache_len = 0;     /* number of terms stored currently    */

    static idx_t last_m = 0;         /* largest m processed so far          */
    static idx_t A      = 0;         /* count of k with {k√2} < 2/3 so far */

    /* Grow cache if it does not contain enough terms. */
    while (cache_len < n) {
        /* Process the next index m = last_m + 1. */
        last_m++;
        idx_t k = last_m;

        /* a = floor(√(2 k²))  */
        uint64_t N = 2ULL * k * k;
        uint64_t a_floor = isqrt_u64(N);

        /* Test: 18 k²  <  (3 a + 2)²  */
        unsigned __int128 lhs = (unsigned __int128)18 * k * k;
        unsigned __int128 rhs = (unsigned __int128)(3 * a_floor + 2);
        rhs = rhs * rhs;
        if (lhs < rhs) {
            /* {k√2} < 2/3 */
            A++;
        }

        /* Check if m satisfies 3*A > 2*m. */
        if ((unsigned __int128)3 * A > (unsigned __int128)2 * last_m) {
            /* Ensure capacity of cache. */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 3 / 2 + 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_m;
        }
    }
    return cache[n - 1];
}
