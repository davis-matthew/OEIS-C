/*
 * A166053  Integers of the form 4n+3 for which Sum_{i=1..u} J(i,4n+3) obtains
 *          value zero exactly 3 times, when u ranges from 1 to (4n+3). Here
 *          J(i,k) is the Jacobi symbol.
 * https://oeis.org/A166053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166053: 4n+3 numbers whose partial sums of Jacobi symbols
   equal zero exactly 3 times for 1 ≤ u < 4n+3. */

#include <stdint.h>
#include <stdlib.h>

static int jacobi_symbol_uint64(uint64_t a, uint64_t n)
{
    /* Computes Jacobi symbol (a/n) for odd n > 0.
       Returns -1, 0, or 1. */
    if ((n & 1ULL) == 0ULL) return 0;          /* n must be odd */
    a %= n;
    int result = 1;
    while (a != 0ULL) {
        /* Factor powers of 2 from a */
        while ((a & 1ULL) == 0ULL) {
            a >>= 1;
            uint64_t r = n & 7ULL;
            if (r == 3ULL || r == 5ULL)
                result = -result;
        }
        /* Swap a and n */
        uint64_t temp = a;
        a = n;
        n = temp;
        /* Legendre symbol reciprocity */
        if ((a & 3ULL) == 3ULL && (n & 3ULL) == 3ULL)
            result = -result;
        a %= n;
    }
    return (n == 1ULL) ? result : 0;
}

static int ok_k(uint64_t k)
{
    int64_t sum = 0;
    int zero_cnt = 0;
    for (uint64_t u = 1; u < k; ++u) {
        int j = jacobi_symbol_uint64(u, k);   /* J(u,k) ∈ {‑1,0,1} */
        sum += j;
        if (sum == 0) {
            ++zero_cnt;
            if (zero_cnt > 3)
                return 0;                     /* too many zeros */
        }
    }
    return zero_cnt == 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static uint64_t next_k = 3;                 /* first candidate 4·0+3 */

    while (len < (size_t)n) {
        uint64_t k = next_k;
        if (ok_k(k)) {
            if (len == cap) {
                size_t newcap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)k;
        }
        next_k += 4;                            /* next 4m+3 candidate */
    }
    return cache[(size_t)n - 1];
}
