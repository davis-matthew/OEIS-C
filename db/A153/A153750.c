/*
 * A153750  Numbers k such that there are 14 digits in k^2 and for each factor
 *          f of 14 (1,2,7) the sum of digit groupings of size f is a square.
 * https://oeis.org/A153750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 266, u64 n <= 266, u128 n <= 266,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153750
 *
 * The sequence consists of all positive integers k such that:
 *   1. k² has exactly 14 decimal digits;
 *   2. For every divisor f of 14 (f = 1,2,7) the sum of the digit
 *      groupings of size f in k² is a perfect square.
 *
 *   • f = 1:  sum of the 14 individual decimal digits is a square;
 *   • f = 2:  split the 14 digits into 7 consecutive 2‑digit numbers
 *      and sum them – the result must be a square;
 *   • f = 7:  split the 14 digits into two consecutive 7‑digit numbers
 *      and sum them – the result must be a square.
 *
 * The offset of the sequence is 1; a(1) is the first term listed
 * in the OEIS entry.  The sequence contains 266 terms, with the last
 * term a(266) = 9996830.
 *
 * The implementation enumerates all integers k whose square has 14
 * digits, tests the three conditions, and stores the qualifying k's
 * in a static array.  Subsequent calls simply index into this array.
 * The routine is deterministic and independent of the calling order.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/* helper: integer square root for uint64_t                         */
/*------------------------------------------------------------------*/
static unsigned long long isqrt_u64(uint64_t n)
{
    unsigned long long lo = 0;
    unsigned long long hi = (1ULL << 32);   /* sqrt(2^64-1) < 2^32            */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        /* use 128‑bit to avoid overflow: mid*mid <= n  */
        if ((__uint128_t)mid * (__uint128_t)mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* helper: test if n is a perfect square (uint64_t)                */
static bool is_square_u64(uint64_t n)
{
    unsigned long long r = isqrt_u64(n);
    return r * r == n;
}

/*------------------------------------------------------------------*/
/* cache for the sequence                                          */
/*------------------------------------------------------------------*/
#define MAX_TERMS 266

static seq_t *terms = NULL;    /* array of terms */
static idx_t  term_count = 0;  /* actual number of stored terms */

/*------------------------------------------------------------------*/
/* build the sequence by scanning all admissible k                */
/*------------------------------------------------------------------*/
static void build_terms(void)
{
    /* lower and upper bounds for k:
     * 10^13 <= k² <= 10^14 - 1                           */
    const uint64_t low_sq  = 10000000000000ULL;          /* 10^13            */
    const uint64_t high_sq = 99999999999999ULL;          /* 10^14 - 1        */

    unsigned long long k_min = isqrt_u64(low_sq);
    if (k_min * k_min < low_sq) k_min++;

    unsigned long long k_max = isqrt_u64(high_sq);

    /* pre-allocate array */
    terms = (seq_t *)malloc(MAX_TERMS * sizeof(seq_t));
    term_count = 0;

    unsigned char d[14];   /* digits of k² */
    for (unsigned long long k = k_min; k <= k_max; ++k) {
        /* compute k² */
        uint64_t sq = k * k;

        /* extract 14 digits */
        uint64_t tmp = sq;
        for (int i = 13; i >= 0; --i) {
            d[i] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }

        /* sum of single digits */
        uint64_t sum1 = 0;
        for (int i = 0; i < 14; ++i) sum1 += d[i];
        if (!is_square_u64(sum1)) continue;

        /* sum of seven 2‑digit numbers */
        uint64_t sum2 = 0;
        for (int i = 0; i < 7; ++i) {
            sum2 += (uint64_t)d[2 * i] * 10 + d[2 * i + 1];
        }
        if (!is_square_u64(sum2)) continue;

        /* sum of two 7‑digit numbers */
        uint64_t val1 = 0, val2 = 0;
        for (int i = 0; i < 7; ++i) {
            val1 = val1 * 10 + d[i];
            val2 = val2 * 10 + d[i + 7];
        }
        uint64_t sum7 = val1 + val2;
        if (!is_square_u64(sum7)) continue;

        /* k satisfies all conditions – store it */
        terms[term_count++] = (seq_t)k;
        if (term_count == MAX_TERMS) break;
    }
}

/*------------------------------------------------------------------*/
/* requested function                                             */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n=0 */
    if (term_count == 0) build_terms();

    if (n - 1 < term_count)
        return terms[n - 1];
    else
        return 0;                  /* undefined beyond known terms */
}
