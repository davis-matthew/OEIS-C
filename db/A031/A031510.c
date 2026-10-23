/*
 * A031510  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 12.
 * https://oeis.org/A031510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that * the continued
 *            fraction of sqrt(k) has an even period, * and when the last term
 *            of its periodic part is removed, * the central term of the
 *            remaining period is 12
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed by a brute‑force search with memoisation. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;   /* sqrt(UINT64_MAX) < 2^32 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Return true if k satisfies the OEIS condition. */
static bool check_k(uint64_t k)
{
    if (k < 2) return false;
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* skip perfect squares */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* Period length is at most 2*sqrt(k) < 20000 for k up to 10^8.
       20000 is ample for all values that the routine will encounter. */
    static uint64_t a_vals[20000];
    size_t period = 0;

    while (1) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;

        if (period < sizeof(a_vals)/sizeof(a_vals[0]))
            a_vals[period] = a;
        ++period;

        /* period terminates when a == 2*a0 (standard property of
           simple continued fractions for sqrt(k)). */
        if (a == 2 * a0) {
            if (period & 1) return false;      /* need even period            */
            /* central term is at index period/2 - 1 (0‑based). */
            uint64_t central = a_vals[period / 2 - 1];
            return central == 12;
        }
    }
    /* unreachable */
    return false;
}

/* --------------------------------------------------------------------- */
/*                     memoised sequence generator                       */
/* --------------------------------------------------------------------- */

#define MAX_TERMS 200000          /* more than enough for n up to ~10 000 */

static seq_t terms[MAX_TERMS];
static idx_t term_count = 0;
static uint64_t next_k = 2;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for zero index – not used in OEIS */

    while (term_count < n) {
        if (check_k(next_k)) {
            if (term_count < MAX_TERMS)
                terms[term_count++] = (seq_t)next_k;
            else
                return 0;  /* safety overflow guard */
        }
        ++next_k;
    }
    return terms[n - 1];
}
