/*
 * A224043  Number of 7 X n 0..1 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>13. Initial values:
 *            a(1)=128, a(2)=987, a(3)=2419, a(4)=4160, a(5)=6321, a(6)=9125,
 *            a(7)=12856, a(8)=17875, a(9)=24623, a(10)=33686, a(11)=45837,
 *            a(12)=62087, a(13)=83746
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 79, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const char *const init_str[14] = {
    "",         /* dummy for 0 index */
    "128",
    "987",
    "2419",
    "4160",
    "6321",
    "9125",
    "12856",
    "17875",
    "24623",
    "33686",
    "45837",
    "62087",
    "83746"
};

static seq_t init_vals[14];
static bool init_done = false;

static void init_constants(void) {
    for (idx_t i = 1; i <= 13; ++i) {
        init_vals[i] = seq_from_str(init_str[i]);
    }
    init_done = true;
}

seq_t a(idx_t n) {
    if (!init_done) init_constants();

    if (n <= 13)       /* use precomputed values */
        return init_vals[n];

    /* last eight terms: last[0] = a(k-1), last[1] = a(k-2), … last[7] = a(k-8) */
    seq_t last[8];
    last[0] = init_vals[13];
    last[1] = init_vals[12];
    last[2] = init_vals[11];
    last[3] = init_vals[10];
    last[4] = init_vals[9];
    last[5] = init_vals[8];
    last[6] = init_vals[7];
    last[7] = init_vals[6];

    for (idx_t k = 14; k <= n; ++k) {
        /* compute next term using the recurrence with only non‑negative arithmetic */
        seq_t pos = 8 * last[0] + 56 * last[2] + 56 * last[4] + 8 * last[6];
        seq_t neg = 28 * last[1] + 70 * last[3] + 28 * last[5] + last[7];
        seq_t next = pos - neg;

        /* shift window */
        last[7] = last[6];
        last[6] = last[5];
        last[5] = last[4];
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = next;
    }

    return last[0];
}
