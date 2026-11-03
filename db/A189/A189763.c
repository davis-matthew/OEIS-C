/*
 * A189763  Even numbers n such that solutions to x' = n are not all
 *          semiprimes, where x' denotes the arithmetic derivative (A003415).
 * https://oeis.org/A189763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth even integer n such that the equation x' = n
 *            (arithmetic derivative) has an anti‑derivative x that is NOT a
 *            semiprime
 * Range:     u8 none, u16 none, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*======================================================================
  Precomputation of all such even numbers up to a set limit.
  For each integer x ≥ 2 we compute its arithmetic derivative
  x' = Σ e_i * x / p_i, where p_i^e_i divides x.
  If x is NOT a semiprime (total multiplicity of primes ≠ 2),
  we store n = x' (if n is even) in a list.  
  Finally we sort, unique the list and keep only the first
  max_terms values.  This list is used for the sequence a(n).
  ====================================================================*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

#define MAX_BUF     500000   /* upper bound for raw derivatives collected */
#define MAX_TERMS   12000    /* number of terms that must be precomputed */

/* Global cache for the sequence values */
static seq_t *seq_vals = NULL;
static size_t seq_count = 0;
static bool initialized = false;

/*======================================================================
  Helper: test whether x is a semiprime
  ====================================================================*/
static bool is_semiprime(uint64_t x)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt, i;
    uint64_t t = x, total = 0;
    if (x < 2) return false;
    factor_u64(t, pr, ex, &cnt);
    for (i = 0; i < cnt; ++i) total += ex[i];
    return total == 2;
}

/*======================================================================
  Helper: compute arithmetic derivative of x
  ====================================================================*/
static uint64_t arithmetic_derivative(uint64_t x)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt, i;
    uint64_t sum = 0;
    factor_u64(x, pr, ex, &cnt);
    for (i = 0; i < cnt; ++i) {
        uint64_t term = ex[i] * (x / pr[i]);
        if (term > UINT64_MAX - sum) return UINT64_MAX; /* overflow guard */
        sum += term;
    }
    return sum;
}

/*======================================================================
  Initialize the sequence cache (called lazily).
  ====================================================================*/
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    seq_t *buf = mem_new(seq_t, MAX_BUF);
    if (!buf) return;          /* out of memory – should not happen here */
    size_t buf_used = 0;

    /* Upper bound for x.  Increase until we have enough terms. */
    uint64_t x = 2;
    uint64_t X_LIMIT = 400000;   /* should be enough for 12k terms */
    while (1) {
        if (x > X_LIMIT) break;
        if (!is_semiprime(x)) {
            uint64_t n = arithmetic_derivative(x);
            if (n <= UINT64_MAX && n % 2 == 0) {
                if (buf_used < MAX_BUF)
                    buf[buf_used++] = (seq_t)n;
            }
        }
        ++x;
    }
    /* Sort and unique the raw list */
    mem_qsort(buf, buf_used, sizeof(seq_t), cmp_seq_t);

    seq_count = 0;
    seq_vals = mem_new(seq_t, MAX_TERMS);
    if (!seq_vals) { mem_free(buf); return; }

    for (size_t i = 0; i < buf_used && seq_count < MAX_TERMS; ++i) {
        if (i == 0 || buf[i] != buf[i - 1]) {
            seq_vals[seq_count++] = buf[i];
        }
    }
    mem_free(buf);
}

/*======================================================================
  The required function a(n).
  ====================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for index 0 */
    init_sequence();
    if ((size_t)n > seq_count) return 0;
    return seq_vals[n - 1];
}
