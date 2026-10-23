/*
 * A194371  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) < 0, where
 *          r=sqrt(3) and fractional part is denoted by < >.
 * https://oeis.org/A194371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer m such that Σ_{k=1..m} (〈1/2 +
 *            k√3〉 - 〈k√3〉) < 0, equivalently 2·#(k≤m, 〈k√3〉 < 1/2) < m
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute integer square root of a 128‑bit unsigned value:
   returns floor(sqrt(x)). */
static inline unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0,
                     hi = ((unsigned __int128)1 << 64) - 1,
                     ans = 0;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Returns true if  〈k√3〉 < 1/2, i.e. if the summand for this k
   contributes +1/2.  The test uses only integer arithmetic. */
static inline bool less_half(idx_t k)
{
    unsigned __int128 t = (unsigned __int128)3 * k * k;      /* 3k²   */
    unsigned __int128 s = isqrt_u128(t);                   /* ⌊k√3⌋  */
    unsigned __int128 left  = (unsigned __int128)12 * k * k;/* 12k²   */
    unsigned __int128 right = (unsigned __int128)4 * s * s + (unsigned __int128)4 * s + 1;
    return left < right;
}

/* Sequence a(n).  The values are generated incrementally and stored
   in a static array so that subsequent calls are fast. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t  terms_cap = 0;
    static size_t  terms_len = 0;
    static idx_t   last_m = 0;   /* last m processed   */
    static idx_t   cnt_pos = 0;  /* number of k≤m with 〈k√3〉 < 1/2 */

    if (!terms) {
        terms_cap = 64;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < n) {
        last_m++;                       /* next m            */
        if (less_half(last_m))
            cnt_pos++;

        /* Condition 2*cnt_pos < last_m  <=> sum < 0 */
        if (((unsigned __int128)2 * cnt_pos) < last_m) {
            if (terms_len >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)last_m;
        }
    }

    return terms[n - 1];
}
