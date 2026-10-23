/*
 * A043491  Numbers having three 0's in base 10.
 * https://oeis.org/A043491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   comb_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose decimal representation
 * contains exactly three zeros.
 * The function uses combinatorial construction to avoid
 * scanning all numbers up to the answer.  It works for
 * any seq_t/idx_t combination and is well under the
 * time limits for n up to thousands. */
#include <stdint.h>
#include "../utils/comb_uint64.h"

/* number = 9^(exp)  (exp <= 20 so it fits 64‑bit) */
static uint64_t pow9[21];

static void init_pow9(void)
{
    pow9[0] = 1;
    for (int i = 1; i < 21; ++i) {
        pow9[i] = pow9[i - 1] * 9ULL;
    }
}

/* count of length L numbers with exactly three zeros */
static uint64_t count_len(int L)
{
    uint64_t c1 = comb_uint64(L, 3);
    uint64_t c2 = comb_uint64(L - 1, 2);
    return (c1 - c2) * pow9[L - 3];
}

/* For the suffix of length rem, with remZeros zeros left */
static uint64_t count_suffix(int rem, int remZeros)
{
    if (remZeros < 0 || remZeros > rem) return 0;
    return comb_uint64(rem, remZeros) * pow9[rem - remZeros];
}

seq_t a(idx_t n)
{
    /* Initialise pow9 once */
    static int once = 0;
    if (!once) { init_pow9(); once = 1; }

    /* Find the length L of the answer */
    int L = 4;
    uint64_t cumulative = 0;
    uint64_t cntL;
    while (1) {
        cntL = count_len(L);
        if (cumulative + cntL >= n) break;
        cumulative += cntL;
        ++L;
    }
    uint64_t offset = n - cumulative; /* 1‑based within this length */

    /* Build the number digit by digit */
    seq_t result = 0;
    int remainingZeros = 3;
    idx_t posOffset = offset - 1; /* 0‑based */
    for (int pos = 0; pos < L; ++pos) {
        int startDigit = (pos == 0) ? 1 : 0;
        for (int d = startDigit; d <= 9; ++d) {
            int z = remainingZeros - (d == 0 ? 1 : 0);
            uint64_t ways = count_suffix(L - pos - 1, z);
            if (posOffset < ways) {
                result = result * 10 + (seq_t)d;
                if (d == 0) --remainingZeros;
                break;
            } else {
                posOffset -= ways;
            }
        }
    }
    return result;
}
