/*
 * A045250  Numbers whose base-5 representation contains no 1's and exactly
 *          three 4's.
 * https://oeis.org/A045250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Computes the n‑th integer whose base‑5 representation * contains
 *            no digit 1 and exactly three digits equal to 4. * The algorithm
 *            determines the necessary digit length by * combinatorial
 *            counting and then builds the number digit by digit
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned long long binom[65][65];
static unsigned long long pow3[65];
static int tables_initialized = 0;

/* Pre‑compute binomial coefficients and powers of 3 up to 64. */
static void init_tables(void)
{
    if (tables_initialized) return;
    /* binomial coefficients */
    for (int i = 0; i <= 64; ++i) {
        binom[i][0] = binom[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
        }
    }
    /* powers of 3 */
    pow3[0] = 1ULL;
    for (int i = 1; i <= 64; ++i) {
        pow3[i] = pow3[i-1] * 3ULL;
    }
    tables_initialized = 1;
}

/* Count for a fixed length L. */
static unsigned long long count_len(int L)
{
    if (L < 3) return 0;
    unsigned long long term1 = 0, term2 = 0;
    if (L-1 >= 2) term1 = binom[L-1][2] * pow3[L-3];
    if (L-1 >= 3) term2 = binom[L-1][3] * 2ULL * pow3[L-4];
    return term1 + term2;
}

/* Count number of ways to fill the remaining `pos` digits
 * when `rem4` fours are still required. */
static unsigned long long count_remaining(int pos, int rem4)
{
    if (rem4 < 0 || rem4 > pos) return 0;
    return binom[pos][rem4] * pow3[pos - rem4];
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_tables();
    idx_t N = n;          /* 1‑based index within the whole sequence */
    int L = 3;            /* minimal possible length */

    /* Find the length L that contains the n‑th term. */
    while (1) {
        unsigned long long cnt = count_len(L);
        if (N <= cnt) break;
        N -= cnt;
        ++L;
    }

    seq_t result = 0;
    int rem4 = 3;            /* fours yet to place */

    /* Construct the number digit by digit (most significant first). */
    for (int i = L-1; i >= 0; --i) {
        const int *digits;
        int ndigits;

        if (i == L-1) {            /* leading digit cannot be 0 */
            static const int dig_main[3] = {2, 3, 4};
            digits = dig_main;
            ndigits = 3;
        } else {
            static const int dig_rest[4] = {0, 2, 3, 4};
            digits = dig_rest;
            ndigits = 4;
        }

        for (int di = 0; di < ndigits; ++di) {
            int d = digits[di];
            int new_rem4 = rem4 - (d == 4 ? 1 : 0);
            unsigned long long cnt = count_remaining(i, new_rem4);

            if ((unsigned long long)N > cnt) {
                N -= (idx_t)cnt;
                continue;
            } else {
                result = result * (seq_t)5 + (seq_t)d;
                rem4 = new_rem4;
                break;
            }
        }
    }

    return result;
}
