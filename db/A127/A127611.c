/*
 * A127611  a(n) = numerator of the continued fraction which has the positive
 *          divisors of n as its terms.
 * https://oeis.org/A127611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, look
 * Formula:   a(n) = numerator of the continued fraction whose terms are the
 *            positive divisors of n in increasing order. The
 *            continued–fraction representation is [1; d2, d3, …, dk] where
 *            d1=1, dk=n are the divisors of n. The numerator is computed by
 *            the standard recurrence p_{-2}=0, p_{-1}=1 p_i = a_i * p_{i-1...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparison function for qsort (works for any seq_t type). */
static int cmp_seq(const void *x, const void *y)
{
    seq_t a = *(const seq_t *)x;
    seq_t b = *(const seq_t *)y;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used (offset starts at 1) */

    /* ---------- factor n by trial division ---------- */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[20];
    int      exp[20];
    int      pcnt = 0;
    uint64_t temp = nn;

    if (temp % 2 == 0) {
        int e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        primes[pcnt] = 2;
        exp[pcnt]    = e;
        ++pcnt;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exp[pcnt++]  = e;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exp[pcnt++]  = 1;
    }

    /* ---------- compute number of divisors ---------- */
    size_t div_cnt = 1;
    for (int i = 0; i < pcnt; ++i) {
        div_cnt *= (exp[i] + 1);
    }

    /* ---------- generate all divisors (unsorted) ---------- */
    seq_t *divs = (seq_t *)malloc(div_cnt * sizeof(seq_t));
    if (!divs) return 0; /* allocation failure: unlikely for small n */

    size_t sz = 1;          /* start with divisor 1 */
    divs[0] = (seq_t)1;

    for (int i = 0; i < pcnt; ++i) {
        uint64_t pval = primes[i];
        int      e    = exp[i];
        size_t   old_sz = sz;

        for (size_t j = 0; j < old_sz; ++j) {
            uint64_t val = (uint64_t)divs[j];
            for (int k = 1; k <= e; ++k) {
                val *= pval;
                divs[sz++] = (seq_t)val;
            }
        }
    }

    /* ---------- sort divisors ascending ---------- */
    qsort(divs, sz, sizeof(seq_t), cmp_seq);

    /* ---------- compute numerator by recurrence ---------- */
    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;
    for (size_t i = 0; i < sz; ++i) {
        seq_t ai     = divs[i];
        seq_t p_curr = ai * p_prev + p_prevprev;
        p_prevprev   = p_prev;
        p_prev       = p_curr;
    }

    seq_t result = p_prev;   /* numerator of the continued fraction */
    free(divs);
    return result;
}
