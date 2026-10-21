/*
 * A226272  Distinct numbers that can be written as u^v, where u and v are not
 *          necessarily distinct digits of n in decimal representation, table
 *          read by rows.
 * https://oeis.org/A226272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6137, u64 n <= 6137,
 *            u128 n <= 6137, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226272:
   The n-th term is the n-th element of the flattened, row-major table
   of distinct values u^v, where u and v are (not necessarily distinct)
   decimal digits of the row index.  Each row is the sorted set of
   values for its index, and the rows are concatenated to form the
   sequence.  This implementation computes the necessary row on demand
   and returns the requested element in O(number_of_rows*digits_per_row)
   time, which is well below the 2‑second limit for n up to ~10000. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t pow_digit[10][10];
static bool pow_init = false;

static void init_pow_table(void) {
    if (pow_init) return;
    for (int base = 0; base <= 9; ++base) {
        for (int exp = 0; exp <= 9; ++exp) {
            if (base == 0 && exp == 0) {
                pow_digit[base][exp] = (seq_t)1;
            } else if (exp == 0) {
                pow_digit[base][exp] = (seq_t)1;
            } else if (base == 0) {
                pow_digit[base][exp] = (seq_t)0;
            } else {
                seq_t val = (seq_t)1;
                for (int i = 0; i < exp; ++i) val *= (seq_t)base;
                pow_digit[base][exp] = val;
            }
        }
    }
    pow_init = true;
}

static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n) {
    init_pow_table();

    idx_t target = n;
    idx_t cum = 0;

    for (idx_t row = 0; ; ++row) {
        /* Extract decimal digits of row */
        uint8_t digits[20];   /* more than enough for 64‑bit numbers */
        size_t dcnt = 0;

        if (row == 0) {
            digits[dcnt++] = 0;
        } else {
            idx_t temp = row;
            while (temp > 0) {
                digits[dcnt++] = (uint8_t)(temp % 10);
                temp /= 10;
            }
        }

        /* Compute distinct u^v values */
        seq_t values[100]; /* maximum 10*10 pairs */
        size_t vcnt = 0;
        for (size_t i = 0; i < dcnt; ++i) {
            int u = digits[i];
            for (size_t j = 0; j < dcnt; ++j) {
                int v = digits[j];
                seq_t val = pow_digit[u][v];
                /* check uniqueness */
                bool exists = false;
                for (size_t k = 0; k < vcnt; ++k) {
                    if (values[k] == val) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    values[vcnt++] = val;
                }
            }
        }

        /* Sort the unique values */
        if (vcnt > 1) qsort(values, vcnt, sizeof(seq_t), cmp_seq);

        if (cum + vcnt <= target) {
            cum += vcnt;
            continue;
        } else {
            idx_t idx = target - cum;
            return values[idx];
        }
    }
}
