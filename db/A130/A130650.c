/*
 * A130650  a(n) = smallest k such that A014612(n+1) = A014612(n) +
 *          (A014612(n) mod k), or 0 if no such k exists.
 * https://oeis.org/A130650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that A014612(n+1) = A014612(n) +
 *            (A014612(n) mod k), 0 if no such k exists
 * Range:     u8 n <= 62, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* globally store the 3-almost-prime sequence (A014612) */
static seq_t *a014 = NULL;          /* array of A014612 values */
static size_t a014_len = 0;         /* number of values stored */
static bool a014_ready = false;     /* initialisation flag */

/* initialise the A014612 array up to a fixed limit (enough for n≈10000) */
static void init_a014(void)
{
    const size_t LIMIT = 200000;          /* upper bound on value to generate */
    int *spf = (int *)malloc((LIMIT + 1) * sizeof(int));
    int *pr = (int *)malloc((LIMIT + 1) * sizeof(int));
    size_t pc = 0;
    /* linear sieve to fill spf[] */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            pr[pc++] = (int)i;
        }
        for (size_t j = 0; j < pc && (size_t)pr[j] <= (size_t)spf[i]
             && i * (size_t)pr[j] <= LIMIT; ++j) {
            spf[i * pr[j]] = pr[j];
        }
    }

    /* gather numbers with exactly 3 prime factors (counted with multiplicity) */
    size_t cap = 50000;
    a014 = (seq_t *)malloc(cap * sizeof(seq_t));
    a014_len = 0;
    for (size_t n = 2; n <= LIMIT; ++n) {
        size_t nn = n;
        size_t omega = 0;
        while (nn > 1) {
            ++omega;
            nn /= spf[nn];
        }
        if (omega == 3) {
            if (a014_len == cap) {
                cap *= 2;
                a014 = (seq_t *)realloc(a014, cap * sizeof(seq_t));
            }
            a014[a014_len++] = (seq_t)n;
        }
    }
    free(spf);
    free(pr);
    a014_ready = true;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (!a014_ready)
        init_a014();

    /* ensure we have n+1 values (indexes are 1-based for A014612) */
    if ((size_t)n + 1 > a014_len)
        return (seq_t)0;   /* insufficient data; should not happen for tested n */

    seq_t first  = a014[n - 1];
    seq_t second = a014[n];
    seq_t delta  = second - first;
    if (delta == 0) return (seq_t)0;

    /* m = first - delta must be > 0 */
    if (first < delta) return (seq_t)0;
    seq_t m = first - delta;

    /* find smallest divisor d of m such that d > delta */
    seq_t best = 0;
    /* m is small (first <= 200000); cast to 64‑bit for sqrt */
    uint64_t m64 = (uint64_t)m;
    uint64_t sqrt_m = (uint64_t)sqrt((double)m64);
    for (uint64_t i = 1; i <= sqrt_m; ++i) {
        if (m64 % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(m64 / i);
            if (d1 > delta && (best == 0 || d1 < best))
                best = d1;
            if (d2 > delta && (best == 0 || d2 < best))
                best = d2;
        }
    }
    return best;
}
