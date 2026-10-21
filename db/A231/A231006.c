/*
 * A231006  Number of months after which a date can fall on the same day of
 *          the week, and the two months can have the same number of days, in
 *          the Gregorian calendar.
 * https://oeis.org/A231006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 0, a(n) = m_i + k*4800 where i = (n-1) mod
 *            count, k = (n-1) / count, and {m_0, …, m_(count-1)} is the
 *            sorted set of positive integers m (1 ≤ m ≤ 4799) for which there
 *            exist two months that are m months apart having the same number
 *            of days and the same weekday for the first day of...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_leap_year
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_leap_year.h"

/* Period of the Gregorian calendar in months */
#define PERIOD 4800

static bool initialized = false;
static seq_t *distances = NULL;    /* sorted list of m values */
static size_t dist_count = 0;     /* number of distances in the list */

/* Precomputes all m in [1, PERIOD-1] that satisfy the property. */
static void compute_distances(void) {
    /* month first weekday and length for months 0 .. PERIOD-1 */
    int first_wd[PERIOD];
    int len[PERIOD];
    int wd = 0;  /* arbitrary starting weekday */

    for (int i = 0; i < PERIOD; ++i) {
        first_wd[i] = wd;
        int m = i % 12;      /* month index 0=Jan … 11=Dec */
        idx_t yr = i / 12;   /* year index */
        int days;

        switch (m) {
            case 0: case 2: case 4: case 6: case 7: case 9: case 11:
                days = 31; break;   /* Jan, Mar, Apr, Jun, Jul, Sep, Oct, Dec */
            case 3: case 5: case 8: case 10:
                days = 30; break;   /* Apr, Jun, Sep, Nov */
            case 1:
                days = is_leap_year(yr) ? 29 : 28;
                break;
            default:
                days = 30; /* should not happen */
        }
        len[i] = days;
        wd = (wd + days) % 7;
    }

    /* For each (length, weekday) group collect indices */
    static int idxs[28][500];   /* 28 groups, max 4800/28≈171 indices */
    static int sz[28];
    for (int g = 0; g < 28; ++g) sz[g] = 0;

    for (int i = 0; i < PERIOD; ++i) {
        int grp = (len[i] - 28) * 7 + first_wd[i];   /* grp in 0..27 */
        idxs[grp][sz[grp]++] = i;
    }

    /* Boolean array to mark which differences appear */
    static bool seen[PERIOD];
    for (int i = 1; i < PERIOD; ++i) seen[i] = false;

    for (int g = 0; g < 28; ++g) {
        int *arr = idxs[g];
        int s = sz[g];
        for (int a = 0; a < s; ++a) {
            for (int b = a + 1; b < s; ++b) {
                int diff = arr[b] - arr[a];
                /* diff is positive and <= PERIOD-1 */
                seen[diff] = true;
            }
        }
    }

    /* Count distances */
    size_t cnt = 0;
    for (int i = 1; i < PERIOD; ++i)
        if (seen[i]) ++cnt;

    /* Allocate and fill sorted list */
    distances = (seq_t *)malloc(cnt * sizeof(seq_t));
    for (int i = 1, j = 0; i < PERIOD; ++i) {
        if (seen[i]) {
            distances[j++] = (seq_t)i;
        }
    }
    dist_count = cnt;
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (!initialized) compute_distances();

    /* n-1 is the index among the list of distances plus multiples */
    idx_t idx = n - 1;
    idx_t block = idx / dist_count;
    idx_t pos   = idx % dist_count;

    seq_t base = distances[pos];
    seq_t add  = (seq_t)block * PERIOD;
    return base + add;
}
