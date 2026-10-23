/*
 * A040486  Continued fraction for sqrt(509).
 * https://oeis.org/A040486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A040486: Continued–fraction terms of √509.
 *
 * a(0) = ⌊√509⌋ = 22
 * a(n) for n≥1 are the periodic part of the simple continued fraction
 * of √509.  The period terminates when the term equals 2·a(0).
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t a0 = 22;                      /* numeric value of ⌊√509⌋ */
static uint64_t a0_num = 22;               /* for arithmetic in initialization */
static seq_t *period = NULL;               /* array of periodic terms (a1 … aL) */
static idx_t period_len = 0, period_cap = 0;/* length and capacity of the array */
static bool initialized = false;           /* flag indicating that the period is ready */

seq_t a(idx_t n)
{
    if (!initialized) {
        /* --- compute the period of the continued fraction --- */
        initialized = true;

        const unsigned long long D = 509ULL;   /* D > a0² */
        unsigned long long m = 0ULL;
        unsigned long long d = 1ULL;
        unsigned long long a_val = a0_num;

        /* Allocate initial capacity */
        period_cap = 16;
        period = (seq_t *)realloc(NULL, period_cap * sizeof(seq_t));
        if (!period) return (seq_t)0;      /* allocation failed */

        while (1) {
            m = d * a_val - m;
            d = (D - m * m) / d;
            a_val = (a0_num + m) / d;

            /* Store the term */
            if (period_len == period_cap) {
                period_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period,
                                               period_cap * sizeof(seq_t));
                if (!tmp) { free(period); return (seq_t)0; }
                period = tmp;
            }
            period[period_len++] = (seq_t)a_val;

            /* Period ends when a == 2·a0 */
            if (a_val == 2ULL * a0_num) break;
        }
    }

    /* --- answer the query --- */
    if (n == 0) return a0;
    return period[(n - 1) % period_len];
}
