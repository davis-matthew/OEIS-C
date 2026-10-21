/*
 * A224200  Number of nX4 0..3 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries 0..3, each row is
 *            unimodal and all antidiagonals are (weakly) nondecreasing.
 *            Computed by dynamic programming over the 130 possible unimodal
 *            rows
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define STATES 130        /* number of unimodal rows of length 4 */

/* All unimodal rows (row index, then column) */
static uint8_t rows[STATES][4];
/* Transition adjacency: row i may be followed by row j iff
   rows[i][0] <= rows[j][1], rows[i][1] <= rows[j][2], rows[i][2] <= rows[j][3] */
static unsigned char trans[STATES][STATES];
static unsigned char trans_len[STATES];

static bool init_done = false;

/* --------------------------------------------------------------------------
 *  Helper: is this a unimodal row of length 4 over 0..3?
 * -------------------------------------------------------------------------- */
static bool is_unimodal(const uint8_t a[4]) {
    /* Find first index where a[i] > a[i+1] (peak starts). */
    int i;
    for (i = 0; i < 3 && a[i] <= a[i + 1]; ++i) {}
    /* From the peak onwards, sequence must not increase. */
    for (int j = i; j < 3; ++j) {
        if (a[j] < a[j + 1]) return false;
    }
    return true;
}

/* --------------------------------------------------------------------------
 *  One‑time initialization: generate all unimodal rows and the
 *  transition matrix.
 * -------------------------------------------------------------------------- */
static void init_once(void) {
    size_t row_cnt = 0;
    /* Enumerate all 4^4 = 256 4‑tuples over 0..3. */
    for (int val = 0; val < 256; ++val) {
        uint8_t a[4];
        int tmp = val;
        for (int k = 3; k >= 0; --k) {
            a[k] = tmp % 4;
            tmp /= 4;
        }
        if (is_unimodal(a)) {
            for (int k = 0; k < 4; ++k) rows[row_cnt][k] = a[k];
            ++row_cnt;
        }
    }
    /* In the official data row_cnt should be 130. */
    (void)row_cnt;   /* silence unused warning in non‑debug builds */

    /* Build transition matrix. */
    for (size_t i = 0; i < STATES; ++i) {
        size_t len = 0;
        for (size_t j = 0; j < STATES; ++j) {
            bool ok = true;
            /* Check the three required inequalities. */
            for (int k = 0; k < 3; ++k) {
                if (rows[j][k] > rows[i][k + 1]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                trans[i][len++] = (unsigned char)j;
            }
        }
        trans_len[i] = (unsigned char)len;
    }

    init_done = true;
}

/* --------------------------------------------------------------------------
 *  public function
 * -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value undefined for n=0 */

    if (!init_done) init_once();

    /* DP vectors for the current and next rows sequences. */
    seq_t current[STATES];
    seq_t next_arr[STATES];

    /* Initial vector for n = 1: one way for each row. */
    for (size_t i = 0; i < STATES; ++i) current[i] = 1;

    if (n == 1) {
        seq_t sum = 0;
        for (size_t i = 0; i < STATES; ++i) sum += current[i];
        return sum;                 /* equals 130 */
    }

    /* Iterate from length 2 up to n. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero next_arr. */
        for (size_t i = 0; i < STATES; ++i) next_arr[i] = 0;

        /* Transition step. */
        for (size_t i = 0; i < STATES; ++i) {
            const seq_t val = current[i];
            if (val == 0) continue;

            size_t t = 0;
            size_t len = trans_len[i];
            while (t < len) {
                size_t j = trans[i][t];
                next_arr[j] += val;
                ++t;
            }
        }

        /* Move next_arr into current for next iteration. */
        for (size_t i = 0; i < STATES; ++i) current[i] = next_arr[i];
    }

    /* Sum of entries of current is a(n). */
    seq_t sum = 0;
    for (size_t i = 0; i < STATES; ++i) sum += current[i];
    return sum;
}
