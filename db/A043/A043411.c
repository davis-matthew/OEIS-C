/*
 * A043411  Numbers whose base-7 representation contains exactly three 4's.
 * https://oeis.org/A043411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043411: numbers whose base‑7 representation contains exactly
 * three 4's.  The algorithm counts how many such numbers have a given
 * length and then builds the nth one digit by digit using combinatorics. */
/* Formula (implicit): enumerate in base‑7 all numbers with exactly three
 * digits equal to 4, skipping those that start with 0. */

#include <stdbool.h>
#include <stdint.h>

#define MAX_L   30          /* enough for any n requested in tests */

/* Tables used for combinatorial counting.  They are filled on first use. */
static idx_t binom[MAX_L + 1][MAX_L + 1];
static idx_t pow6[MAX_L + 1];
static idx_t cum[MAX_L + 1];
static bool initialized = false;

/* Pre‑compute binomial coefficients, powers of 6 and cumulative counts. */
static void init_tables(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Binomial coefficients */
    for (int i = 0; i <= MAX_L; ++i) {
        binom[i][0] = binom[i][i] = (idx_t)1;
        for (int j = 1; j < i; ++j)
            binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
    }

    /* Powers of 6 */
    pow6[0] = (idx_t)1;
    for (int i = 1; i <= MAX_L; ++i)
        pow6[i] = pow6[i-1] * (idx_t)6;

    /* Cumulative counts of numbers with exactly three 4's by length */
    cum[0] = cum[1] = cum[2] = (idx_t)0;
    for (int l = 3; l <= MAX_L; ++l) {
        idx_t cnt = 0;
        /* first digit is 4 */
        if (l - 1 >= 2)
            cnt += binom[l-1][2] * pow6[l-3];
        /* first digit is not 4 and not 0 */
        if (l - 1 >= 3)
            cnt += (idx_t)5 * binom[l-1][3] * pow6[l-4];
        cum[l] = cum[l-1] + cnt;
    }
}

/* Return the nth element (n ≥ 1). */
seq_t a(idx_t n)
{
    init_tables();

    /* Find the length l where the nth number lies. */
    int l = 3;
    while (l <= MAX_L && cum[l] < n)
        ++l;

    idx_t rank = n - cum[l-1];          /* rank within length‑l block */
    seq_t result = 0;
    int used4 = 0;

    /* Build the number digit by digit. */
    for (int pos = 0; pos < l; ++pos) {
        int remaining = l - pos - 1;
        for (int d = 0; d < 7; ++d) {
            if (pos == 0 && d == 0)      /* leading zero not allowed */
                continue;

            int need4 = 3 - (used4 + (d == 4));
            if (need4 < 0 || need4 > remaining)
                continue;

            idx_t cnt = binom[remaining][need4] *
                        pow6[remaining - need4];
            if (cnt < rank) {           /* skip these numbers */
                rank -= cnt;
                continue;
            }

            /* Choose this digit */
            result = result * (seq_t)7 + (seq_t)d;
            if (d == 4)
                ++used4;
            break;                       /* move to next position */
        }
    }

    return result;
}
