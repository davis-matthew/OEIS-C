/*
 * A087727  Triangle read by rows: T(n,k) = (2 * (binomial(n,k)) * (n + 2 * k
 *          + 3)!)/((k + 1)! * (k + 2)! * (n + 3)!).
 * https://oeis.org/A087727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: T(n,k) = (2 * binomial(n,k) * (n + 2*k + 3)!)/((k + 1)! * (k + 2)! * (n + 3)!),
 * with a(0)=1 and a(n) read by rows:
 * row = largest integer r such that r(r+1)/2 <= n,
 * col = n - r(r+1)/2.
 */
 
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t prime_max = 0;

/* ensure we have all primes up to limit */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_list && prime_max >= limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_max = limit;
}

static void add_factor(idx_t val, int *exps, size_t pc, int sign)
{
    while (val > 1) {
        /* find prime factor */
        size_t lo = 0, hi = pc;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] <= val)
                lo = mid + 1;
            else
                hi = mid;
        }
        /* prime_list[lo-1] is the largest prime <= val */
        if (lo == 0) /* val itself is prime and > prime_list[pc-1]? but safe */
            return;
        uint64_t p = prime_list[lo - 1];
        if (p * p > val) {
            /* val is prime */
            /* binary search to find its index */
            lo = 0; hi = pc;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (prime_list[mid] < val)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            if (lo < pc && prime_list[lo] == val) {
                exps[lo] += sign;
            }
            break;
        }
        while (val % p == 0) {
            /* find index of p */
            for (size_t i = 0; i < pc; ++i) {
                if (prime_list[i] == p) {
                    exps[i] += sign;
                    break;
                }
            }
            val /= p;
        }
    }
}

/* Add prime exponents of val with given sign (+1 or -1) */
static void add_exp_value(idx_t val, int *exps, size_t pc, int sign)
{
    if (val <= 1)
        return;
    for (size_t pi = 0; pi < pc; ++pi) {
        uint64_t p = prime_list[pi];
        if (p * p > val)
            break;
        while (val % p == 0) {
            exps[pi] += sign;
            val /= p;
        }
    }
    if (val > 1) {
        /* val is prime */
        size_t lo = 0, hi = pc;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] < val)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo < pc && prime_list[lo] == val)
            exps[lo] += sign;
    }
}

seq_t a(idx_t n)
{
    /* compute row and column via triangular indexing */
    idx_t row = 0;
    while (row * (row + 1) / 2 <= n)
        row++;
    row--;
    idx_t col = n - row * (row + 1) / 2;

    /* maximal value needed for prime generation */
    idx_t max_val = row + 2 * col + 3;
    if (max_val < 3)
        max_val = 3;
    ensure_primes_upto(max_val);

    /* exponent array for prime factors */
    static int exp[200];      /* primes up to ~400 < 200 */
    size_t pc = prime_cnt;
    for (size_t i = 0; i < pc; ++i)
        exp[i] = 0;

    /* numerator: factor 2 */
    for (size_t i = 0; i < pc; ++i) {
        if (prime_list[i] == 2) {
            exp[i] += 1;
            break;
        }
    }

    /* binomial numerator: (row - col + i) for i=1..col */
    for (idx_t i = 1; i <= col; ++i) {
        idx_t val = row - col + i;
        add_exp_value(val, exp, pc, +1);
    }

    /* binomial denominator: i for i=1..col */
    for (idx_t i = 1; i <= col; ++i) {
        idx_t val = i;
        add_exp_value(val, exp, pc, -1);
    }

    /* factorial ratio numerator: product i = row+4 .. row+2*col+3 */
    if (col > 0) {
        for (idx_t i = row + 4; i <= row + 2 * col + 3; ++i) {
            idx_t val = i;
            add_exp_value(val, exp, pc, +1);
        }
    }

    /* factorial(k+1)! denominator: i=1..col+1 */
    for (idx_t i = 1; i <= col + 1; ++i) {
        idx_t val = i;
        add_exp_value(val, exp, pc, -1);
    }

    /* factorial(k+2)! denominator: i=1..col+2 */
    for (idx_t i = 1; i <= col + 2; ++i) {
        idx_t val = i;
        add_exp_value(val, exp, pc, -1);
    }

    /* compute the result from prime exponents */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < pc; ++pi) {
        int e = exp[pi];
        for (int i = 0; i < e; ++i)
            result *= (seq_t)prime_list[pi];
    }

    return result;
}
