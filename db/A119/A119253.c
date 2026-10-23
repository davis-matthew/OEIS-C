/*
 * A119253  Odd digits in increasing order.
 * https://oeis.org/A119253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the nth smallest integer whose decimal digits are odd and
 *            strictly increasing. (There are 31 such numbers, corresponding
 *            to all non‑empty subsets of {1,3,5,7,9}.)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t seq[31];
static bool init = false;

static void generate_sequence(void)
{
    size_t count = 0;
    for (int mask = 1; mask < 32; ++mask) {        /* all non‑empty subsets of 5 digits */
        seq_t num = 0;
        for (int i = 0; i < 5; ++i) {             /* digits 1,3,5,7,9 in order */
            if (mask & (1 << i)) {
                num = num * 10 + (seq_t)(1 + 2 * i);
            }
        }
        seq[count++] = num;
    }

    /* simple insertion/swap sort (31 elements only) */
    for (size_t i = 0; i < 31; ++i) {
        for (size_t j = i + 1; j < 31; ++j) {
            if (seq[j] < seq[i]) {
                seq_t tmp = seq[i];
                seq[i] = seq[j];
                seq[j] = tmp;
            }
        }
    }

    init = true;
}

seq_t a(idx_t n)
{
    if (!init) {
        generate_sequence();
    }
    if (n < 1 || n > 31) {
        return 0;
    }
    return seq[(size_t)(n - 1)];
}
