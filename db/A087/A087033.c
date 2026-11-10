/*
 * A087033  Number of terms in each group of consecutive zeros in A087032.
 * https://oeis.org/A087033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = length of the n-th maximal run of zeros in A087032
 *            (A087032(m)=1 iff 2*nextprime(m)-m is prime); computed by
 *            scanning A087032 with cached state
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-07)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

static uint32_t *A087033_runs = NULL;
static size_t A087033_cnt = 0, A087033_cap = 0;
static uint64_t A087033_m = 1;
static uint32_t A087033_cur = 0;

static int A087033_f(uint64_t m)
{
    uint64_t p = next_prime(m);
    uint64_t x = 2 * p - m;
    return is_prime_u64(x) ? 1 : 0;
}

seq_t a(idx_t n)
{
    while (A087033_cnt < (size_t)n) {
        int v = A087033_f(A087033_m++);
        if (v == 0) {
            A087033_cur++;
        } else if (A087033_cur > 0) {
            if (A087033_cnt == A087033_cap) {
                size_t nc = A087033_cap ? A087033_cap * 2 : 1024;
                A087033_runs = (uint32_t *)realloc(A087033_runs, nc * sizeof(uint32_t));
                A087033_cap = nc;
            }
            A087033_runs[A087033_cnt++] = A087033_cur;
            A087033_cur = 0;
        }
    }
    return (seq_t)A087033_runs[(size_t)n - 1];
}
