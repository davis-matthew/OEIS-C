/*
 * A040725  Continued fraction for sqrt(753).
 * https://oeis.org/A040725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040725 – Continued fraction coefficients for sqrt(753)
   a(0) = 27
   for n >= 1, a(n) is the (n-1)-th partial quotient of the repeating block. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- Helper: integer square root for 64‑bit values ----- */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* ----- Period cache (store terms of the repeating block) ----- */
static uint64_t *period_buf = NULL;   /* array of period terms */
static size_t    period_len = 0;      /* number of terms in the period */
static size_t    period_cap = 0;      /* capacity of period_buf */

/* Append a value to the dynamic period array. */
static void add_period_val(uint64_t val)
{
    if (period_len == period_cap) {
        size_t new_cap = period_cap ? period_cap * 2u : 64u;
        uint64_t *tmp = (uint64_t *)realloc(period_buf, new_cap * sizeof(uint64_t));
        if (!tmp) abort();               /* allocation failed */
        period_buf = tmp;
        period_cap = new_cap;
    }
    period_buf[period_len++] = val;
}

/* Compute all terms of the repeating block of the continued fraction of sqrt(753). */
static void compute_period(void)
{
    if (period_len > 0) return;  /* already computed */

    const uint64_t D = 753ULL;
    uint64_t a0 = isqrt_uint64(D);      /* floor(sqrt(753)) = 27 */
    /* Classic continued‑fraction recurrence */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        add_period_val(a);
        if (a == 2 * a0) break;        /* 2*a0 marks the end of the period */
    }
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    /* Lazily compute the period on first use. */
    if (period_len == 0) compute_period();

    if (n == 0) {
        /* a(0) = floor(sqrt(753)) = 27 */
        return (seq_t)27;
    }

    /* Periodic part: index (n-1) modulo period length. */
    size_t idx = (size_t)((n - 1) % period_len);
    return (seq_t)period_buf[idx];
}
