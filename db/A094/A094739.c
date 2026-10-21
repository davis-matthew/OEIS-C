/*
 * A094739  Numbers m such that 4^k*m, for integer k >= 0, are numbers having
 *          a unique partition into three squares.
 * https://oeis.org/A094739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094739: numbers m such that 4^k*m, for k ≥ 0,
 * are numbers having a unique partition into three squares.
 *
 * Algorithm:
 *   – For each integer m ≥ 1 (skipping multiples of 4) we
 *     check whether it admits exactly one decomposition
 *     m = x^2 + y^2 + z^2 with 0 ≤ x ≤ y ≤ z.
 *   – All such m are collected until the known total of 33
 *     terms is reached.  The sequence is known to contain
 *     exactly 33 members, so no further search is required.
 *
 * The test for a unique partition enumerates all possible
 * pairs (x , y).  For each pair it computes
 *   z² = m – x² – y²
 * and checks whether z is an integer and z ≥ y.
 * If more than one admissible triple is found,
 * the function returns false.
 *
 * To avoid dependence on external utilities we implement
 * an integer square‑root routine that operates on 64‑bit
 * values (all terms fit into 64 bits).
 *
 * The result is cached so that repeated calls to a(idx_t n)
 * have O(1) runtime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- integer square root for 64‑bit values ---------------------------- */
static seq_t
isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x)  { lo = mid + 1; ans = mid; }
        else         { if (mid == 0) break; hi = mid - 1; }
    }
    return (seq_t)ans;
}

/* --- test whether m has exactly one representation as a sum of
 *      three squares with 0 ≤ x ≤ y ≤ z ---------------------------- */
static bool
has_unique_partition(seq_t m)
{
    uint64_t val   = (uint64_t)m;
    uint64_t mx    = (uint64_t)isqrt_uint64(val);
    int       cnt  = 0;

    for (uint64_t x = 0; x <= mx; ++x) {
        uint64_t xsq = x * x;
        if (xsq > val) break;
        uint64_t rem_yz = val - xsq;
        uint64_t my    = (uint64_t)isqrt_uint64(rem_yz);

        for (uint64_t y = x; y <= my; ++y) {
            uint64_t ysq = y * y;
            if (xsq + ysq > val) break;
            uint64_t z2   = rem_yz - ysq;
            uint64_t z    = (uint64_t)isqrt_uint64(z2);
            if (z * z != z2) continue;
            if (z < y)       continue;
            ++cnt;
            if (cnt > 1) return false;   /* more than one representation */
        }
    }
    return cnt == 1;
}

/* --- generate the 33 known terms once -------------------------------- */
#define TOTAL_TERMS 33

static seq_t *terms     = NULL;
static idx_t   terms_cnt = 0;
static bool    ready     = false;

static void
generate_terms(void)
{
    size_t cap = 64;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) exit(1);

    seq_t m = 1;
    while (true) {
        if ((m & 3) != 0) {                 /* m must not be divisible by 4 */
            if (has_unique_partition(m)) {
                if (terms_cnt >= cap) {
                    cap <<= 1;
                    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                    if (!terms) exit(1);
                }
                terms[terms_cnt++] = m;
                if (terms_cnt == TOTAL_TERMS) break;
            }
        }
        ++m;  /* all terms fit into the seq_t range we are searching */
    }
    ready = true;
}

/* ---------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (!ready) generate_terms();
    if (n == 0   || n > terms_cnt) return (seq_t)0;
    return terms[n - 1];   /* sequence is 1–indexed */
}
