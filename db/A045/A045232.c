/*
 * A045232  Numbers whose base-5 representation contains exactly three 1's and
 *          three 2's.
 * https://oeis.org/A045232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest integer whose base‑5 representation
 *            contains exactly three 1's and three 2's. The base‑5 digits may
 *            be 0, 3 or 4 in any other positions. The function enumerates all
 *            such numbers in increasing order and returns the n‑th term (a(1)
 *            is the smallest). The implementation uses...
 * Range:     u8 none, u16 n <= 400, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t comb[21][21];
static uint64_t pow3[21];
static int comb_pow3_initialized = 0;

static void init_comb_pow3(void)
{
    if (comb_pow3_initialized) return;
    for (int i = 0; i <= 20; ++i) {
        comb[i][0] = comb[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            comb[i][j] = comb[i - 1][j - 1] + comb[i - 1][j];
        }
    }
    pow3[0] = 1;
    for (int i = 1; i <= 20; ++i) pow3[i] = pow3[i - 1] * 3;
    comb_pow3_initialized = 1;
}

/* Count of base‑5 numbers of exactly length L that contain
   exactly three 1’s and three 2’s, and whose first digit is non‑zero. */
static uint64_t count_len(int L)
{
    if (L < 6) return 0;
    uint64_t total = comb[L][3] * comb[L - 3][3] * pow3[L - 6];
    if (L >= 7) {
        uint64_t leading = comb[L - 1][3] * comb[(L - 1) - 3][3];
        int free = (L - 1) - 3 - 3;               /* positions after a leading 0 */
        if (free > 0) leading *= pow3[free];
        total -= leading;                       /* exclude leading‑zero sequences */
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    init_comb_pow3();

    /* Find the length L of the n‑th number. */
    idx_t cum = 0;
    int L = 6;
    while (true) {
        uint64_t cnt = count_len(L);
        if ((size_t)cum + cnt >= (size_t)n) break;
        cum += cnt;
        ++L;
    }

    /* 0‑based index within all numbers of length L. */
    uint64_t idx = (uint64_t)(n - cum - 1);

    /* Unrank the idx‑th string of length L. */
    int remPos = L;
    int rem1 = 3;            /* remaining 1’s to place */
    int rem2 = 3;            /* remaining 2’s to place */
    int digits[20];          /* maximum L is < 20 for n up to 10000 */

    for (int pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 4; ++d) {
            if (pos == 0 && d == 0) continue;               /* no leading zero */
            if (d == 1 && rem1 == 0) continue;
            if (d == 2 && rem2 == 0) continue;

            int remPos1 = remPos - 1;
            int r1 = rem1 - (d == 1);
            int r2 = rem2 - (d == 2);
            if (r1 < 0 || r2 < 0) continue;
            int free = remPos1 - r1 - r2;
            if (free < 0) continue;

            uint64_t cnt = comb[remPos1][r1] *
                           comb[remPos1 - r1][r2] *
                           pow3[free];

            if (cnt <= idx) {
                idx -= cnt;
                continue;
            } else {
                digits[pos] = d;
                remPos = remPos1;
                rem1 = r1;
                rem2 = r2;
                break;
            }
        }
    }

    /* Convert base‑5 digits to decimal. */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 5 + (seq_t)digits[i];
    }
    return value;
}
