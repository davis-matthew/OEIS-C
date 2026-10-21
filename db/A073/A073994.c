/*
 * A073994  Number of strings of length n over Z_6 with trace 3 and subtrace
 *          5.
 * https://oeis.org/A073994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A073994:
   a(n; t, s) = Σ_{d=0..5} a(n‑1; (t‑d) mod6 , (s – d·(t‑d)) mod6)
   with all arithmetic on t and s performed modulo 6.
   Base: a(1; t, 0) = 1  for all t in {0,…,5}.  (Only one string of length 1
   has subtrace 0.)
   We store the 36 possible (t,s) values in a one‑dimensional table:
   table[n * 36 + t * 6 + s]   (n ≥ 1).  The function returns a(n)=a(n;3,5). */

#include <stdlib.h>

#define STATES 36
#define TRACE 6
#define SUBTRACE 6
#define TARGET_T 3
#define TARGET_S 5

/* Convert (t,s) to a flat index 0…35. */
static inline int index_ts(int t, int s) {
    return t * SUBTRACE + s;
}

/* Static cache for previously computed rows. */
static idx_t max_n_computed = 0;
static seq_t *table = NULL;

/* Main function. */
seq_t a(idx_t n) {
    if (n < 1)                      /* offset 1 – below not used */
        return 0;

    /* Expand the cache if necessary. */
    if (n > max_n_computed) {
        size_t old_max = (size_t)max_n_computed;
        size_t new_max = (size_t)n;

        if (table == NULL) {        /* first allocation: also set base row */
            table = (seq_t *)malloc((new_max + 1) * STATES * sizeof(seq_t));
            if (table == NULL)      /* allocation failed */
                return 0;

            /* initialise all entries to 0 */
            for (size_t i = 0; i < STATES; ++i)
                table[1 * STATES + i] = (seq_t)0;

            /* base row: a(1; t,0)=1 for all t */
            for (int t = 0; t < TRACE; ++t)
                table[1 * STATES + index_ts(t, 0)] = (seq_t)1;

            old_max = 1;
        } else {                    /* grow the existing table */
            seq_t *tmp = (seq_t *)realloc(table,
                                          (new_max + 1) * STATES * sizeof(seq_t));
            if (tmp == NULL)        /* allocation failed */
                return 0;
            table = tmp;
        }

        /* Compute any missing rows. */
        for (size_t cur = old_max + 1; cur <= new_max; ++cur) {
            for (int t = 0; t < TRACE; ++t) {
                for (int s = 0; s < SUBTRACE; ++s) {
                    seq_t sum = 0;
                    for (int d = 0; d < TRACE; ++d) {
                        int tprev = (t - d + TRACE) % TRACE;           /* (t‑d) mod6 */
                        int tmp = (d * tprev) % TRACE;                 /* d·(t‑d) mod6 */
                        int sprev = (s - tmp + TRACE) % TRACE;         /* (s‑tmp) mod6 */
                        sum += table[(cur - 1) * STATES + index_ts(tprev, sprev)];
                    }
                    table[cur * STATES + index_ts(t, s)] = sum;
                }
            }
        }
        max_n_computed = new_max;
    }

    /* Return the requested value a(n)=a(n;3,5). */
    return table[n * STATES + index_ts(TARGET_T, TARGET_S)];
}
