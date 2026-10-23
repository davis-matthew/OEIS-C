/*
 * A045063  Numbers whose base-4 representation contains no 0's and exactly
 *          two 3's.
 * https://oeis.org/A045063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045063: a(n) = n-th smallest integer whose base‑4 representation
   contains no 0's and exactly two 3's. Indexing starts at 1. */

#include <stdint.h>
#include <stdbool.h>

static idx_t comb(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static idx_t pow2(int e)
{
    return (idx_t)1 << e;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the length L of the base‑4 representation */
    idx_t cumulative = 0;
    int L = 2;          /* minimal length (needs two 3's) */
    idx_t prev_cumulative = 0;
    while (1) {
        idx_t local = comb(L, 2) * pow2(L - 2);
        if (cumulative + local >= n) break;
        cumulative += local;
        ++L;
    }
    idx_t r = n - cumulative;          /* 1‑based rank within length L */

    /* Build the digits of the r‑th number */
    uint8_t digits[64];
    int remaining_3 = 2;
    idx_t rank = r;
    for (int pos = 0; pos < L; ++pos) {
        for (uint8_t d = 1; d <= 3; ++d) {
            if (d == 3 && remaining_3 == 0) continue;   /* cannot use more 3's */
            int new_rem = remaining_3 - (d == 3);
            int R = L - pos - 1;                       /* remaining positions */
            if (new_rem > R) continue;                 /* impossible */

            idx_t cnt = comb(R, new_rem) * pow2(R - new_rem);
            if (cnt < rank) {
                rank -= cnt;
                continue;
            }
            digits[pos] = d;
            remaining_3 = new_rem;
            break;
        }
    }

    /* Convert base‑4 digits to decimal */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 4 + (seq_t)digits[i];
    }
    return value;
}
