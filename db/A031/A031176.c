/*
 * A031176  Periods of sum of squares of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = period of the orbit of n under the
 *            sum‑of‑squares‑of‑digits map
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum possible sum of squares of the digits for any 64‑bit integer. */
#define MAX_SS 1620

/* sum of squares of digits of a 64‑bit integer */
static uint64_t digit_square_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;            /* 0 maps to 0 */
}

/* period cache for numbers 0 .. MAX_SS.  0 means “unknown”. */
static seq_t period[MAX_SS + 1] = {0};

/* initialize the period table once */
static void init_period_table(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* clear the table */
    for (int i = 0; i <= MAX_SS; ++i) period[i] = 0;

    int order[MAX_SS + 1];
    int path[MAX_SS + 1];

    for (int i = 0; i <= MAX_SS; ++i) {
        if (period[i] != 0) continue;   /* already known */

        /* reset visitation markers */
        for (int j = 0; j <= MAX_SS; ++j) order[j] = -1;

        int cur = i;
        int path_len = 0;
        seq_t cycle_len = 0;

        while (1) {
            if (cur <= MAX_SS && period[cur] != 0) {   /* reached known node */
                cycle_len = period[cur];
                break;
            }
            if (order[cur] != -1) {                   /* found loop in this path */
                cycle_len = (seq_t)(path_len - order[cur]);
                break;
            }
            order[cur] = path_len;
            path[path_len++] = cur;

            uint64_t next = digit_square_sum_uint64((uint64_t)cur);
            cur = (int)next;   /* next is <= MAX_SS */
        }

        /* assign the period to all nodes visited in this path */
        for (int k = 0; k < path_len; ++k)
            period[path[k]] = cycle_len;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_period_table();

    if (n <= (idx_t)MAX_SS) {
        return period[(int)n];
    } else {
        /* for larger n, the first iteration already falls into the range
           0 .. MAX_SS, so the period depends only on that value. */
        uint64_t first = digit_square_sum_uint64((uint64_t)n);
        return period[(int)first];
    }
}
