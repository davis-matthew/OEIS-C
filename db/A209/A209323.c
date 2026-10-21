/*
 * A209323  Values of omega(n) (A001221) as n runs through the triprimes
 *          (A014612).
 * https://oeis.org/A209323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω( T(n) ), where T(n) is the nth integer that has exactly
 *            three prime factors (counted with multiplicity). The function
 *            enumerates all such integers up to a fixed limit on the first
 *            call, stores their distinct‑prime counts, and returns the stored
 *            value for each n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* One‑time initialization of the table of triprimes */
    static bool init = false;
    static seq_t *vals = NULL;   /* stores ω(T(k)) for k≥1 */
    static size_t count = 0;

    if (!init)
    {
        /* LIMIT large enough to contain the first 10000 triprimes. */
        const idx_t LIMIT   = 400000;   /* safe upper bound */
        const size_t INIT_CAP = 512;

        /* ---- build smallest‑prime‑factor array via linear sieve ---- */
        idx_t *spf = (idx_t *)calloc((size_t)LIMIT + 1, sizeof(idx_t));
        idx_t *pr  = (idx_t *)malloc((size_t)LIMIT * sizeof(idx_t));
        size_t pcnt = 0;

        for (idx_t i = 2; i <= LIMIT; ++i)
        {
            if (spf[i] == 0) {
                spf[i] = i;
                pr[pcnt++] = i;
            }
            for (size_t j = 0; j < pcnt; ++j) {
                idx_t p = pr[j];
                idx_t prod = i * p;
                if (p > spf[i] || prod > LIMIT) break;
                spf[prod] = p;
            }
        }
        free(pr);

        /* ---- collect triprimes and their ω values ---- */
        vals = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
        size_t cap = INIT_CAP;
        count = 0;

        for (idx_t x = 2; x <= LIMIT; ++x)
        {
            /* count Ω(x) */
            idx_t tmp = x;
            int omega = 0;
            while (tmp > 1) {
                ++omega;
                tmp /= spf[tmp];
            }
            if (omega != 3) continue;      /* not a tripprime */

            /* count ω(x) (distinct prime factors) */
            tmp = x;
            int distinct = 0;
            int last_p = 0;
            while (tmp > 1) {
                int p = (int)spf[tmp];
                if (p != last_p) {
                    ++distinct;
                    last_p = p;
                }
                tmp /= p;
            }

            /* store result */
            if (count >= cap) {
                cap *= 2;
                vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
            }
            vals[count++] = (seq_t)distinct;
        }

        free(spf);
        init = true;
    }

    /* n is 1‑based; return the stored value or 0 if out of range */
    if (n == 0 || n > count) return 0;
    return vals[(size_t)n - 1];
}
