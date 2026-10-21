/*
 * A064931  Numbers m such that the digits of m are also digits of m^3.
 * https://oeis.org/A064931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(n) = the n‑th integer m (1‑indexed) such that for every
 *            decimal * digit d, the number of occurrences of d in m^3 is at
 *            least the number * of occurrences of d in m. * (OEIS A064931)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 363, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *terms = NULL;
    static size_t term_count = 0;   /* how many terms we have stored */
    static size_t term_cap   = 0;   /* allocated capacity of terms */
    /* next integer to test */
    static seq_t next_candidate = 1;
    /* initialisation flag */
    static bool initialized = false;

    if (!initialized) {
        term_cap = 128;
        terms = (seq_t*)malloc(term_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;          /* OOM – return 0 */
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (term_count < (size_t)n) {
        seq_t m = next_candidate;

        /* count digits of m */
        unsigned char cnt_m[10] = {0};
        seq_t x = m;
        if (x == 0) cnt_m[0]++;
        while (x) {
            unsigned d = (unsigned)(x % (seq_t)10);
            ++cnt_m[d];
            x /= (seq_t)10;
        }

        /* compute cube m^3 */
        seq_t cube = m;
        cube *= m;
        cube *= m;

        /* count digits of cube */
        unsigned char cnt_c[10] = {0};
        x = cube;
        if (x == 0) cnt_c[0]++;
        while (x) {
            unsigned d = (unsigned)(x % (seq_t)10);
            ++cnt_c[d];
            x /= (seq_t)10;
        }

        /* test condition that every digit in m appears at least as often in cube */
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt_m[d] > cnt_c[d]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* store term */
            if (term_count == term_cap) {
                term_cap *= 2;
                seq_t *new_terms = (seq_t*)realloc(terms, term_cap * sizeof(seq_t));
                if (!new_terms) return (seq_t)0;  /* OOM – return 0 */
                terms = new_terms;
            }
            terms[term_count++] = m;
        }

        /* advance to next integer for the next iteration */
        next_candidate++;
    }

    return terms[n - 1];
}
