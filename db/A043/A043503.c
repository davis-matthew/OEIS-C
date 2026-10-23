/*
 * A043503  Numbers having three 3's in base 10.
 * https://oeis.org/A043503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the n-th positive integer whose decimal representation contains
 *            exactly three digits equal to 3 (OEIS A043503)
 * Range:     u8 none, u16 n <= 698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Table of binomial coefficients up to 10 */
static seq_t binom[11][11];
/* Powers of 9: 9^k */
static seq_t pow9[11];
/* Flag whether tables have been initialized */
static bool tables_inited = false;

/* Lazily initialise binomial coefficients and powers of 9 */
static void init_tables(void)
{
    if (tables_inited) return;
    for (idx_t i = 0; i <= 10; ++i)
    {
        for (idx_t j = 0; j <= i; ++j)
        {
            if (j == 0 || j == i)
                binom[i][j] = (seq_t)1;
            else
                binom[i][j] = binom[i - 1][j - 1] + binom[i - 1][j];
        }
    }
    pow9[0] = (seq_t)1;
    for (idx_t i = 1; i <= 10; ++i)
        pow9[i] = pow9[i - 1] * 9;
    tables_inited = true;
}

/* Count D‑digit numbers (most significant digit non‑zero) with exactly
   three occurrences of the digit 3 */
static seq_t count_dig_digits(idx_t D)
{
    if (D == 3)          /* only 333 */
        return (seq_t)1;
    if (D < 3)
        return (seq_t)0;

    /* part1: leading digit is 3, choose 2 of the remaining D-1 digits to be 3 */
    seq_t part1 = binom[D - 1][2] * pow9[D - 3];
    /* part2: leading digit not 3 (8 choices), choose 3 of the remaining D-1 digits to be 3 */
    seq_t part2 = 8 * binom[D - 1][3] * pow9[D - 4];
    return part1 + part2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();       /* ensure tables are ready */

    /* Determine the number of digits D of the n-th term */
    idx_t D = 3;
    seq_t cumulative = 0;
    seq_t count_D = 0;
    while (true)
    {
        count_D = count_dig_digits(D);
        if (cumulative + count_D >= n) break;
        cumulative += count_D;
        ++D;
    }

    /* r – rank within the D‑digit block (1‑based) */
    seq_t r = n - cumulative;

    seq_t result = 0;
    idx_t used_3 = 0;

    for (idx_t pos = 0; pos < D; ++pos)
    {
        int digit_start = (pos == 0) ? 1 : 0;  /* leading zero not allowed */
        for (int d = digit_start; d <= 9; ++d)
        {
            idx_t new_used_3 = used_3 + (d == 3);
            if (new_used_3 > 3) continue;      /* cannot exceed three 3's */

            idx_t remaining = D - pos - 1;          /* digits yet to place */
            idx_t need = 3 - new_used_3;            /* remaining 3's required */

            if (need > remaining) continue;        /* impossible to satisfy */

            seq_t cnt = binom[remaining][need] * pow9[remaining - need];
            if (cnt >= r)
            {
                result = result * 10 + d;
                used_3 = new_used_3;
                break;
            }
            else
            {
                r -= cnt;
            }
        }
    }
    return result;
}
