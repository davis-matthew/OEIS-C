/*
 * A122467  Write n-th semiprime in binary, sum as if decimal numbers.
 * https://oeis.org/A122467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = SUM_{i=1..n} (decimal representation of binary(i-th
 *            semiprime))
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum is of the decimal value obtained by interpreting the binary
 * digits of each semiprime as decimal digits.  The array of prefix sums is
 * built lazily and memoised, so repeated calls to a() are fast.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* the build provides the types seq_t and idx_t; we do not redeclare them */
/* static state for memoisation */
static uint32_t *spf = NULL;          /* least prime factor array */
static idx_t    spf_limit = 0;        /* current maximum number for spf */

static seq_t   *prefix = NULL;        /* prefix sums: prefix[0] = 0 */
static size_t   prefix_cap = 0;       /* allocated capacity for prefix */

static idx_t    cur_val = 1;          /* next candidate to test for semiprime */
static idx_t    max_computed_n = 0;   /* how many terms have been computed */
static seq_t    current_sum = 0;      /* running sum of decimal values */

/* ------------------------------------------------------------------ */
/* Reallocate the spf array to hold values up to 'limit' (inclusive).   */
static void compute_full_spf(idx_t limit)
{
    uint32_t *tmp = (uint32_t *)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
    spf = tmp;
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* Ensure the sieve covers at least 'needed'.  Double growth for efficiency. */
static void ensure_spf_at_least(idx_t needed)
{
    if (needed <= spf_limit)
        return;
    idx_t new_limit = spf_limit == 0 ? 1024 : spf_limit * 2;
    if (new_limit < needed * 2)
        new_limit = needed * 2;
    compute_full_spf(new_limit);
}

/* ------------------------------------------------------------------ */
/* Simple test whether x is a semiprime using the pre‑computed spf array. */
static bool is_semiprime(idx_t x)
{
    if (x < 4) return false;
    int count = 0;
    idx_t t = x;
    while (t > 1 && count <= 2) {
        t /= spf[t];
        ++count;
    }
    return (count == 2 && t == 1);
}

/* Number of bits needed to represent x (assumes x > 0). */
static inline int bitlen_idx(idx_t x)
{
    return x == 0 ? 0 : 64 - __builtin_clzll((unsigned long long)x);
}

/* Interpret the binary digits of x as decimal digits (e.g. 110₂ -> 110₁₀). */
static seq_t binary_to_decimal(idx_t x)
{
    seq_t result = 0;
    int bits = bitlen_idx(x);
    for (int i = bits - 1; i >= 0; --i) {
        result = result * 10 + ((x >> i) & 1);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Ensure the prefix array has space for 'needed' entries. */
static void grow_prefix(size_t needed)
{
    if (needed <= prefix_cap)
        return;
    size_t new_cap = prefix_cap ? prefix_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
    prefix = tmp;
    prefix_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute terms up to at least index n, storing prefix sums. */
static void ensure_precomputed(idx_t n)
{
    if (n <= max_computed_n)
        return;

    if (prefix == NULL) {
        grow_prefix(16);
        prefix[0] = 0;                     /* sum of 0 terms */
    }

    while (max_computed_n < n) {
        /* Find next semiprime. */
        do {
            ++cur_val;
            if (cur_val > spf_limit)
                ensure_spf_at_least(cur_val * 2 + 1024);
        } while (!is_semiprime(cur_val));

        seq_t dec_val = binary_to_decimal(cur_val);
        current_sum += dec_val;

        ++max_computed_n;
        grow_prefix(max_computed_n + 1);   /* +1 because prefix[0] is used */
        prefix[max_computed_n] = current_sum;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) is undefined in OEIS; return 0 safely */
        return 0;
    ensure_precomputed(n);
    return prefix[n];
}
