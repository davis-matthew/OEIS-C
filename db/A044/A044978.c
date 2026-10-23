/*
 * A044978  Numbers whose base-3 representation contains exactly two 0's and
 *          three 1's.
 * https://oeis.org/A044978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑3 representation contains exactly
 *            two 0's and three 1's, enumerated by increasing length and
 *            lexicographic order
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Pre‑compute small binomial coefficients */
static seq_t comb[20][20];

static void init_combs(void)
{
    static int done = 0;
    if (done) return;
    for (int i = 0; i < 20; ++i) {
        comb[i][0] = 1;
        comb[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            comb[i][j] = comb[i-1][j-1] + comb[i-1][j];
        }
    }
    done = 1;
}

/* Count of valid strings of length L (first digit ≠ 0) with two 0's
   and three 1's. */
static seq_t valid_count(int L)
{
    /* L >= 5.  Choose the two zero positions from {1,…,L−1}
       then the three ones from the remaining L−2 positions. */
    return comb[L-1][2] * comb[L-2][3];
}

/* Number of ways to fill the remaining positions after pos
   (positions pos+1 … L‑1) with the given number of zeros and ones. */
static seq_t ways_remaining(int pos, int L, int zeros, int ones)
{
    int rem = L - pos - 1; /* positions left after current */
    if (zeros < 0 || ones < 0) return 0;
    if (zeros + ones > rem) return 0;
    return comb[rem][zeros] * comb[rem - zeros][ones];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, return 0 */
    init_combs();

    idx_t rem = n;          /* 1‑based position within the current length */
    int L = 5;              /* minimal length that allows two 0's and three 1's */

    while (1) {
        seq_t cnt = valid_count(L);
        if (rem <= (idx_t)cnt) break;   /* found the needed length */
        rem -= (idx_t)cnt;               /* skip whole block of this length */
        ++L;
    }

    seq_t rank = (seq_t)(rem - 1);   /* zero‑based rank within this length block */
    int zeros = 2, ones = 3;
    uint8_t digits[20] = {0};        /* enough for maximum L (~12) */

    for (int pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 2; ++d) {
            if (pos == 0 && d == 0) continue;                /* no leading zero */
            if (d == 0 && zeros == 0) continue;
            if (d == 1 && ones == 0) continue;

            int z = zeros - (d == 0);
            int o = ones  - (d == 1);
            seq_t w = ways_remaining(pos, L, z, o);

            if (rank < w) {
                digits[pos] = (uint8_t)d;
                zeros = z;
                ones  = o;
                break;
            } else {
                rank -= w;
            }
        }
    }

    seq_t value = 0;
    for (int pos = 0; pos < L; ++pos) {
        value = value * 3 + (seq_t)digits[pos];
    }
    return value;
}
