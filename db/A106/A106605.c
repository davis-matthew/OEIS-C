/*
 * A106605  Vertically symmetrical dates DDMMYYYY excluding years which are
 *          divisible by 10, considered as numbers, in increasing order.
 * https://oeis.org/A106605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   days_in_month, is_leap_year
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerates all dates and returns vertically symmetrical
   DDMMYYYY values, sorted by numeric value, excluding years divisible
   by 10. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/days_in_month.h"
#include "../utils/is_leap_year.h"

/* mirror mapping for calculator display: 0→0,1→1,2→5,5→2,8→8 */
static const int8_t MIRROR[10] = {
    0, 1, 5, -1, -1, 2, -1, -1, 8, -1
};

/* test vertical symmetry of a date DD MM YYYY. */
static inline bool vertical_symmetry(int dd, int mm, int yyyy) {
    int d[8];
    d[0] = dd / 10;          d[1] = dd % 10;
    d[2] = mm / 10;          d[3] = mm % 10;
    d[4] = (yyyy / 1000) % 10;
    d[5] = (yyyy / 100) % 10;
    d[6] = (yyyy / 10) % 10;
    d[7] = yyyy % 10;

    for (int i = 0; i < 4; ++i) {
        int r = d[7 - i];
        int m = MIRROR[r];
        if (m == -1 || d[i] != m) return false;
    }
    return true;
}

/* comparison function for qsort on seq_t */
static int cmp_seq(const void *a, const void *b) {
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* static storage for the sequence, large enough for all terms */
#define MAX_TERMS 200
static seq_t seq_terms[MAX_TERMS];
static idx_t seq_count = 0;
static bool seq_computed = false;

static void compute_sequence(void) {
    if (seq_computed) return;
    seq_computed = true;

    for (int year = 1; year <= 9999; ++year) {
        if (year % 10 == 0) continue;           /* skip years divisible by 10 */

        for (int month = 1; month <= 12; ++month) {
            int dim = days_in_month(year, month);
            for (int day = 1; day <= dim; ++day) {
                if (!vertical_symmetry(day, month, year)) continue;
                seq_t val = (seq_t)day * (seq_t)1000000u
                          + (seq_t)month * (seq_t)10000u
                          + (seq_t)year;
                if (seq_count < MAX_TERMS) {
                    seq_terms[seq_count++] = val;
                }
            }
        }
    }

    /* sort by numeric value to obtain OEIS order */
    qsort(seq_terms, seq_count, sizeof(seq_t), cmp_seq);
}

/* public function */
seq_t a(idx_t n) {
    compute_sequence();
    if (n == 0) return 0;                 /* undefined for offset 0 */
    if (n - 1 < seq_count) return seq_terms[n - 1];
    return 0;                               /* out of range */
}
