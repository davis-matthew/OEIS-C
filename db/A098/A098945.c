/*
 * A098945  Numbers where 1 is the only odd decimal digit.
 * https://oeis.org/A098945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 3798, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: generate the n‑th positive integer whose decimal
 * digits are chosen from the set {0,1,2,4,6,8} and that contains at
 * least one '1'.  The sequence is sorted in ascending numeric order. */

#include <stddef.h>

static seq_t pow6[20];
static seq_t pow5[20];
static int precomputed = 0;

/* Maps for digit choices */
static const int lead_map[5] = {1, 2, 4, 6, 8};
static const int digit_map[6] = {0, 1, 2, 4, 6, 8};

seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for n < 1 */

    /* Precompute powers of 6 and 5 up to a safe bound */
    if (!precomputed) {
        pow6[0] = 1;
        pow5[0] = 1;
        for (int i = 1; i < 20; ++i) {
            pow6[i] = pow6[i - 1] * 6;
            pow5[i] = pow5[i - 1] * 5;
        }
        precomputed = 1;
    }

    /* Determine the length L of the nth number */
    seq_t cumulative = 0;
    int L = 1;
    for (;;) {
        seq_t countL = 5 * pow6[L - 1] - 4 * pow5[L - 1];
        if (cumulative + countL >= n)
            break;
        cumulative += countL;
        ++L;
    }

    /* Index within the sequence of length L (0‑based) */
    seq_t offset = n - cumulative - 1;

    int digits[20];
    int seen1 = 0;

    /* Position 0 : choose the leading digit (cannot be 0) */
    for (int li = 0; li < 5; ++li) {
        int d = lead_map[li];
        int new_seen = seen1 || (d == 1);
        int rem = L - 1;          /* remaining positions after this one */
        seq_t block;
        if (new_seen)
            block = pow6[rem];
        else
            block = pow6[rem] - pow5[rem];
        if (offset < block) {
            digits[0] = d;
            seen1 = new_seen;
            break;
        }
        offset -= block;
    }

    /* Remaining positions */
    for (int pos = 1; pos < L; ++pos) {
        for (int di = 0; di < 6; ++di) {
            int d = digit_map[di];
            int new_seen = seen1 || (d == 1);
            int rem = L - pos - 1;
            seq_t block;
            if (new_seen)
                block = pow6[rem];
            else
                block = pow6[rem] - pow5[rem];
            if (offset < block) {
                digits[pos] = d;
                seen1 = new_seen;
                break;
            }
            offset -= block;
        }
    }

    /* Convert digit array to the numeric result */
    seq_t result = 0;
    for (int i = 0; i < L; ++i) {
        result = result * 10 + digits[i];
    }
    return result;
}
