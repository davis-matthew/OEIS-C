/*
 * A053289  First differences of consecutive perfect powers (A001597).
 * https://oeis.org/A053289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001597(n+1) - A001597(n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A053289: first differences of consecutive perfect powers. */
#include <stdlib.h>
#include <stdbool.h>

static int seq_compare(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t *powers = NULL;
    static size_t count = 0;
    static bool ready = false;

    if (!ready) {
        /* Upper bound for the (10 000)-th perfect power is below 1e8.
           Generate all perfect powers ≤ 1e8. */
        const seq_t LIMIT = 100000000ULL;      /* 1e8 */
        const idx_t MAX_BASE = 10000;          /* sqrt(LIMIT) */
        size_t capacity = MAX_BASE + MAX_BASE / 2 + 16;  /* rough upper bound */

        powers = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!powers) exit(1);  /* allocation failed */

        size_t sz = 0;
        powers[sz++] = (seq_t)1;     /* include 1 as a perfect power */

        for (idx_t base = 2; base <= MAX_BASE; ++base) {
            seq_t pow = (seq_t)base * base;   /* base^2 */
            while (pow <= LIMIT) {
                powers[sz++] = pow;
                /* compute next exponent, guard against overflow */
                if (pow > LIMIT / base) break;
                pow *= (seq_t)base;
            }
        }

        /* Sort the list */
        qsort(powers, sz, sizeof(seq_t), seq_compare);

        /* Remove duplicates (e.g., 64 = 8² = 4³) */
        size_t uniq = 0;
        for (size_t i = 0; i < sz; ++i) {
            if (i == 0 || seq_compare(&powers[i], &powers[i - 1]) != 0) {
                powers[uniq++] = powers[i];
            }
        }
        count = uniq;
        ready = true;
    }

    /* The array contains at least n+1 perfect powers for n ≤ 10000. */
    return powers[n] - powers[n - 1];
}
