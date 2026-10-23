/*
 * A031604  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 16.
 * https://oeis.org/A031604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A031604:
   numbers k such that the continued fraction for sqrt(k) has odd
   period and, after deleting the last term of that period, the two
   central terms of the remaining even‑length sequence are both 16.
   The function generates the sequence on demand, memoizing results.
   The results are returned as type seq_t, suitable for 8–128 bit
   unsigned integers or arbitrary precision. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit unsigned integers */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;          /* sqrt(2^64-1) = 2^32-1 */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ------------------------------------------------------------------ */
/* Period storage (dynamic array of uint32_t)                          */
static uint32_t *period_arr = NULL;
static size_t   period_cap = 0;

static void ensure_period_cap(size_t need)
{
    if (need <= period_cap) return;
    size_t new_cap = period_cap ? period_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(period_arr, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);
    period_arr = tmp;
    period_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Result storage (dynamic array of seq_t)                             */
static seq_t *results = NULL;
static size_t results_cap = 0;
static idx_t  results_len = 0;
static idx_t  next_candidate = 2;      /* start scanning from 2 */

static void ensure_results_cap(size_t need)
{
    if (need <= results_cap) return;
    size_t new_cap = results_cap ? results_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    results = tmp;
    results_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Helper: test a single candidate k */
static bool qualifies_k(uint64_t k)
{
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* perfect square, skip */

    uint64_t m = 0, d = 1, a = a0;
    size_t len = 0;

    while (true) {
        uint64_t m_next = d * a - m;
        uint64_t d_next = (k - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        ensure_period_cap(len + 1);
        period_arr[len++] = (uint32_t)a_next;

        /* End of period when the last term equals 2*a0 */
        if (a_next == (2 * a0))
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* Period length must be odd */
    if ((len & 1) == 0) return false;

    /* Special case: period length 1. Accept only if the only term is 16. */
    if (len == 1)
        return period_arr[0] == 16;

    /* For len >= 3 : after deletion of the last term, the remaining
       even-length sequence has two central terms that must both be 16. */
    size_t mid_minus = (len - 1) / 2 - 1;
    size_t mid_plus  = (len - 1) / 2;
    return period_arr[mid_minus] == 16 && period_arr[mid_plus] == 16;
}

/* ------------------------------------------------------------------ */
/* Public function as required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Undefined, but harmless */

    while (results_len < n) {
        uint64_t cand = next_candidate++;
        if (qualifies_k(cand)) {
            ensure_results_cap(results_len + 1);
            results[results_len++] = (seq_t)cand;
        }
    }
    return results[n - 1];
}
