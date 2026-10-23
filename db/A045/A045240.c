/*
 * A045240  Numbers whose base-5 representation contains exactly two 1's and
 *          no 3's.
 * https://oeis.org/A045240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly two 1’s and
 *            no 3’s. The sequence is indexed starting at 1
 * Range:     u8 n <= 36, u16 n <= 4154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) */
#include <stdbool.h>

#define MAX_LEN 120          /* sufficient for all practical n */

static bool initialized = false;
static seq_t pow3[MAX_LEN + 1];
static seq_t comb[MAX_LEN + 1][MAX_LEN + 1];
static seq_t counts[MAX_LEN + 1];
static seq_t cumulative[MAX_LEN + 1];

/* Initialise combinatorial tables once */
static void init_tables(void)
{
    /* powers of 3 */
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_LEN; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* binomial coefficients */
    for (idx_t i = 0; i <= MAX_LEN; ++i) {
        comb[i][0] = comb[i][i] = (seq_t)1;
        for (idx_t k = 1; k < i; ++k)
            comb[i][k] = comb[i - 1][k - 1] + comb[i - 1][k];
    }

    /* counts for each length */
    counts[0] = counts[1] = (seq_t)0;
    cumulative[0] = cumulative[1] = (seq_t)0;
    for (idx_t L = 2; L <= MAX_LEN; ++L) {
        seq_t case1 = ((seq_t)(L - 1)) * pow3[L - 2];
        seq_t case2 = (seq_t)0;
        if (L >= 3)
            case2 = ((seq_t)2) * comb[L - 1][2] * pow3[L - 3];
        counts[L] = case1 + case2;
        cumulative[L] = cumulative[L - 1] + counts[L];
    }

    initialized = true;
}

/* Main function returning the n-th term (n >= 1) */
seq_t a(idx_t n)
{
    if (!initialized)
        init_tables();

    /* Find the length containing the n-th element */
    idx_t L = 1;
    seq_t n_seq = (seq_t)n;
    while (L <= MAX_LEN && cumulative[L] < n_seq)
        ++L;

    /* 0‑based index within numbers of length L */
    seq_t idx = n_seq - cumulative[L - 1] - (seq_t)1;
    seq_t number = (seq_t)0;
    idx_t rem_ones = (idx_t)2;

    for (idx_t pos = 1; pos <= L; ++pos) {
        /* allowed digits for this position in increasing order */
        int digits[4];
        idx_t nd = 0;
        if (pos == 1) {
            digits[nd++] = 1;
            digits[nd++] = 2;
            digits[nd++] = 4;
        } else {
            digits[nd++] = 0;
            digits[nd++] = 1;
            digits[nd++] = 2;
            digits[nd++] = 4;
        }

        for (idx_t di = 0; di < nd; ++di) {
            int d = digits[di];
            if (d == 1 && rem_ones == 0)
                continue;          /* cannot use more than two 1’s */

            idx_t new_rem1 = rem_ones - (d == 1 ? 1 : 0);
            idx_t remaining = L - pos;

            seq_t ways = (seq_t)0;
            if (new_rem1 <= remaining) {
                ways = comb[remaining][new_rem1] * pow3[remaining - new_rem1];
            }

            if (idx < ways) {
                /* choose this digit */
                number = number * (seq_t)5 + (seq_t)d;
                rem_ones = new_rem1;
                break;
            } else {
                idx -= ways;
            }
        }
    }

    return number;
}
