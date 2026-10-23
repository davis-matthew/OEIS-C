/*
 * A194395  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) < 0, where
 *          r=sqrt(14) and < > denotes fractional part.
 * https://oeis.org/A194395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th integer m such that
 * Sum_{k=1..m} ( {1/2 + k*sqrt(14)} - {k*sqrt(14)} ) < 0
 * The summand is +0.5 if the fractional part of k*sqrt(14) is < 1/2,
 * otherwise –0.5.  Thus we keep a running sum of signs +1 / –1
 * and record every m for which that sum becomes negative.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 128‑bit unsigned integer: floor(sqrt(x)). */
static uint64_t isqrt_u128(__uint128_t n)
{
    uint64_t low = 0;
    uint64_t high = ~0ULL;           /* 2^64-1, the maximum 64‑bit value */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t midsq = (__uint128_t)mid * mid;
        if (midsq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;     /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t processed_m = 0;       /* the smallest m that has not yet
                                           been processed */
    static int64_t partial_sum = 0;     /* running sum of +1 / -1 signs */
    static size_t terms_cnt = 0;        /* number of terms currently stored */
    static size_t terms_cap = 0;        /* allocated capacity of terms array */
    static seq_t *terms = NULL;         /* array of m where partial_sum < 0 */

    /* generate terms until we have at least n of them */
    while (terms_cnt < n) {
        processed_m++;               /* next candidate m */

        /* compute floor(k*sqrt(14)) using integer sqrt:
           f = floor(sqrt(14) * k) = floor(sqrt(14*k^2))            */
        __uint128_t sq14 = (__uint128_t)14 * processed_m * processed_m;
        uint64_t f = isqrt_u128(sq14);

        /* convenient left/right comparison:
           56*k^2 < (2*f+1)^2  => sign +1, else sign -1             */
        __uint128_t left = (__uint128_t)56 * processed_m * processed_m;
        __uint128_t right = (__uint128_t)(2ULL * f + 1);
        right = right * right;
        int sign = (left < right) ? 1 : -1;

        partial_sum += sign;

        if (partial_sum < 0) {
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms,
                                              terms_cap * sizeof(seq_t));
                if (!tmp) { /* simple out‑of‑memory guard */
                    abort();
                }
                terms = tmp;
            }
            terms[terms_cnt++] = (seq_t)processed_m;
        }
    }

    return terms[n - 1];
}
