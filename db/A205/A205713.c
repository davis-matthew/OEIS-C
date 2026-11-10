/*
 * A205713  The number j such that 9 divides prime(k)-prime(j), where
 *          k(n)=A205712(n).
 * https://oeis.org/A205713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. List the primes in order. For each k, write down every j
 *            < k with prime(j) == prime(k) (mod 9), smallest j first. a(n) is
 *            the n-th j written
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A205713_cum = NULL;   /* cum[k-1]: total terms through prime index k */
static unsigned char *A205713_res = NULL;
static size_t A205713_cnt = 0, A205713_cap = 0;
static uint64_t *A205713_lst[9];
static size_t A205713_lcnt[9], A205713_lcap[9];
static uint64_t A205713_last = 1;

seq_t a(idx_t n)
{
    while (A205713_cnt == 0 || A205713_cum[A205713_cnt - 1] < (uint64_t)n) {
        uint64_t c = A205713_last + 1;
        while (!is_prime_u64(c)) ++c;
        A205713_last = c;
        if (A205713_cnt + 1 > A205713_cap) {
            A205713_cap = A205713_cap ? A205713_cap * 2 : 256;
            A205713_cum = (uint64_t *)realloc(A205713_cum, A205713_cap * sizeof(uint64_t));
            A205713_res = (unsigned char *)realloc(A205713_res, A205713_cap);
            if (!A205713_cum || !A205713_res) exit(EXIT_FAILURE);
        }
        unsigned r = (unsigned)(c % 9);
        uint64_t prev = A205713_cnt ? A205713_cum[A205713_cnt - 1] : 0;
        A205713_cum[A205713_cnt] = prev + (uint64_t)A205713_lcnt[r];
        A205713_res[A205713_cnt] = (unsigned char)r;
        if (A205713_lcnt[r] + 1 > A205713_lcap[r]) {
            A205713_lcap[r] = A205713_lcap[r] ? A205713_lcap[r] * 2 : 64;
            A205713_lst[r] = (uint64_t *)realloc(A205713_lst[r], A205713_lcap[r] * sizeof(uint64_t));
            if (!A205713_lst[r]) exit(EXIT_FAILURE);
        }
        A205713_lst[r][A205713_lcnt[r]++] = (uint64_t)A205713_cnt + 1;
        A205713_cnt++;
    }
    size_t lo = 0, hi = A205713_cnt - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A205713_cum[mid] >= (uint64_t)n) hi = mid; else lo = mid + 1;
    }
    uint64_t prev = lo ? A205713_cum[lo - 1] : 0;
    uint64_t off = (uint64_t)n - prev - 1;
    return (seq_t)A205713_lst[A205713_res[lo]][off];
}
