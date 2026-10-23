/*
 * A042371  Denominators of continued fraction convergents to sqrt(712).
 * https://oeis.org/A042371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction of sqrt(712) Recurrence: a(0)=1; for n>=1 let p_n be
 *            the nth partial quotient after a0 (a(0)=1, a(1)=p1, a(2)=p2, …).
 *            The denominators q_n are given by q_(-1)=0, q_0=1, q_k =
 *            p_k*q_(k-1) + q_(k-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *period_terms = NULL;  /* periodic partial quotients after a0 */
static size_t period_len = 0;
static seq_t *denoms = NULL;           /* cached denominators q_k  (q_0 = 1) */
static size_t denoms_len = 0;
static size_t denoms_cap = 0;
static bool initialized = false;

/* compute the period of the continued fraction of sqrt(712) */
static void compute_period(void) {
    uint64_t D = 712;
    uint64_t a0 = 26;          /* floor(sqrt(712)) */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    size_t cap = 16;
    period_terms = (uint64_t *)malloc(cap * sizeof(uint64_t));

    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        if (period_len == cap) {
            cap *= 2;
            period_terms = (uint64_t *)realloc(period_terms, cap * sizeof(uint64_t));
        }
        period_terms[period_len++] = a;   /* store the partial quotient */
    } while (a != 2 * a0);
}

/* extend the denoms array up to index n (inclusive) */
static void ensure_size(idx_t n) {
    if (n < denoms_len)
        return;

    if (!initialized) {
        compute_period();
        denoms_cap = 64;
        denoms = (seq_t *)malloc(denoms_cap * sizeof(seq_t));
        denoms[0] = (seq_t)1;   /* q_0 = 1 */
        denoms_len = 1;
        initialized = true;
    }

    while (denoms_len <= (size_t)n) {
        size_t k = denoms_len;                       /* index of the next denominator to compute */
        uint64_t period_index = (k - 1) % period_len;
        seq_t p_k = (seq_t)period_terms[period_index];
        seq_t q_prev1 = denoms[denoms_len - 1];
        seq_t q_prev2 = (denoms_len >= 2) ? denoms[denoms_len - 2] : (seq_t)0;
        seq_t q_k = p_k * q_prev1 + q_prev2;

        if (denoms_len >= denoms_cap) {
            denoms_cap *= 2;
            denoms = (seq_t *)realloc(denoms, denoms_cap * sizeof(seq_t));
        }
        denoms[denoms_len++] = q_k;
    }
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return denoms[(size_t)n];
}
