/*
 * A218523  Number of 2Xn arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..3 2Xn array.
 * https://oeis.org/A218523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+17*a(n-2)+26*a(n-3)-19*a(n-4)-123*a(n-5)-138*a(n-6) *
 *            -248*a(n-7)-329*a(n-8)+41*a(n-9)+1108*a(n-10)+1469*a(n-11) *
 *            -54*a(n-12)-1158*a(n-13)-316*a(n-14)+144*a(n-15)+32*a(n-16) *
 *            for n > 16; a(1)=16, a(2)=36, a(3)=286, a(4)=1348, a(5)=7902,
 *            a(6)=42812, * a(7)=242438, a(8)=1349524, a...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[16] = {
    16ULL, 36ULL, 286ULL, 1348ULL, 7902ULL, 42812ULL,
    242438ULL, 1349524ULL, 7567448ULL, 42328892ULL,
    237017542ULL, 1326679924ULL, 7427188036ULL,
    41577119948ULL, 232754004212ULL, 1302972323234ULL
};

static const int coeffs[16] = {
    2, 17, 26, -19, -123, -138, -248, -329,
    41, 1108, 1469, -54, -1158, -316, 144, 32
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* initialise cache with first 16 terms */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_terms[i];
        cache_len = 16;
    }

    /* extend cache up to requested n */
    while (cache_len < n) {
        seq_t next = 0;
        idx_t curIdx = cache_len;  /* 0‑based index for a(n) to be computed */

        for (int i = 0; i < 16; ++i) {
            idx_t idx = curIdx - i - 1;        /* index for a(n-(i+1)) */
            seq_t prev = cache[idx];
            int c = coeffs[i];
            if (c > 0) {
                next += prev * (seq_t)c;
            } else {
                next -= prev * (seq_t)(-c);
            }
        }

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
