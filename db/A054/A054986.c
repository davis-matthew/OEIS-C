/*
 * A054986  Base-10 modest numbers.
 * https://oeis.org/A054986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   Compute the sequence of base‑10 modest numbers by brute force. A
 *            number x is modest if there is a split of its decimal
 *            representation into a first part A and a second part B (B may
 *            have leading zeros) with B > 0 and x % B == A. The function
 *            stores the positive modest numbers in the order they are...
 * Range:     u8 n <= 24, u16 n <= 1284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Static state shared by all calls */
#ifdef __cplusplus
static std::vector<seq_t> modest_vec;        /* list of found modest numbers */
#else
static seq_t *modest_arr = NULL;             /* array of found modest numbers */
static size_t modest_len = 0;                /* current count */
static size_t modest_cap = 0;                /* allocated capacity */
#endif

static seq_t next_candidate = (seq_t)1;       /* next integer to test */

/* Ensure that the C array has at least 'need' capacity */
#ifndef __cplusplus
static void ensure_capacity(size_t need)
{
    if (modest_cap >= need) return;
    size_t new_cap = modest_cap == 0 ? 4 : modest_cap * 2;
    while (new_cap < need) new_cap *= 2;
    void *p = realloc(modest_arr, new_cap * sizeof(seq_t));
    modest_arr = (seq_t *)p;
    modest_cap = new_cap;
}
#endif

/* Append a new modest number to the list */
static void append_modest(seq_t v)
{
#ifdef __cplusplus
    modest_vec.push_back(v);
#else
    ensure_capacity(modest_len + 1);
    modest_arr[modest_len++] = v;
#endif
}

/* Test whether a given integer x is modest */
static bool is_modest(seq_t x)
{
    /* A single digit number cannot be split */
    if (x < (seq_t)10) return false;

    /* Count decimal digits of x by repeated division by 10 */
    int digits = 0;
    seq_t temp = x;
    do {
        ++digits;
        temp /= (seq_t)10;
    } while (temp > (seq_t)0);

    /* Test every possible split position */
    seq_t pow10 = (seq_t)10;      /* 10^1 */
    for (int k = 1; k < digits; ++k) {
        seq_t suffix = x % pow10;
        if (suffix != (seq_t)0) {
            seq_t prefix = x / pow10;
            if (x % suffix == prefix) return true;
        }
        pow10 *= (seq_t)10;
    }
    return false;
}

/* Main function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but guard */

#ifdef __cplusplus
    size_t attained = modest_vec.size();
#else
    size_t attained = modest_len;
#endif

    /* Generate modest numbers until we have at least n of them */
    while (attained < (size_t)n) {
        if (is_modest(next_candidate)) {
            append_modest(next_candidate);
            ++attained;
        }
        ++next_candidate;
    }

#ifdef __cplusplus
    return modest_vec[(size_t)n - 1];
#else
    return modest_arr[(size_t)n - 1];
#endif
}
