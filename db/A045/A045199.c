/*
 * A045199  Numbers whose base-5 representation contains exactly three 0's and
 *          no 3's.
 * https://oeis.org/A045199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑5 representation contains exactly
 * three 0's and no 3's.  The n‑th term (offset 1) is returned. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helper functions for combinatorics ---------- */

/* compute binomial coefficient C(n,k) using 128‑bit arithmetic */
static unsigned __int128 binom(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 r = 1;
    for (int i = 1; i <= k; ++i) {
        r = r * (unsigned __int128)(n - k + i) / (unsigned __int128)i;
    }
    return r;
}

/* pre‑computed powers of 3 ( 3^e ) for e up to 64 */
static unsigned __int128 pow3[65] = {0};

static void init_pow3(void) {
    if (pow3[0] == 0) {               /* one‑time init */
        pow3[0] = 1;
        for (int i = 1; i < 65; ++i)
            pow3[i] = pow3[i - 1] * (unsigned __int128)3;
    }
}

/* count of admissible numbers of length len (base‑5) */
static unsigned __int128 count_len(int len) {
    if (len < 4) return 0;
    return binom(len - 1, 3) * pow3[len - 3];
}

/* ---------- main function ---------- */

seq_t a(idx_t n) {
    init_pow3();

    /* 1. Find the length L of the n‑th number */
    unsigned __int128 cum = 0;
    int L = 3;
    unsigned __int128 target = (unsigned __int128)n;

    while (true) {
        ++L;                                /* start from 4 */
        unsigned __int128 cnt = count_len(L);
        if (cum + cnt >= target) break;
        cum += cnt;
    }

    unsigned __int128 offset = target - cum; /* 1‑based index within length L */

    /* 2. Unrank the offset to obtain the specific digits */
    uint8_t digits[64];
    int remainingZeros = 3;
    int positionsLeft = L;

    for (int pos = 0; pos < L; ++pos) {
        /* choose digit in ascending order */
        int digitList[4];
        int listSize = 0;
        if (pos == 0) {                          /* first digit cannot be 0 */
            digitList[0] = 1; digitList[1] = 2; digitList[2] = 4;
            listSize = 3;
        } else {
            digitList[0] = 0; digitList[1] = 1;
            digitList[2] = 2; digitList[3] = 4;
            listSize = 4;
        }

        for (int d = 0; d < listSize; ++d) {
            int dig = digitList[d];
            if (dig == 0 && remainingZeros == 0) continue;

            int remZeros = remainingZeros - (dig == 0 ? 1 : 0);
            int remLen   = positionsLeft - 1;
            unsigned __int128 cnt = binom(remLen, remZeros) * pow3[remLen - remZeros];

            if (offset > cnt) {
                offset -= cnt;
                continue;
            }

            /* this digit is chosen */
            digits[pos] = (uint8_t)dig;
            remainingZeros = remZeros;
            positionsLeft = remLen;
            break;
        }
    }

    /* 3. Convert digits to decimal value in seq_t */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * (seq_t)5 + (seq_t)digits[i];
    }
    return value;
}
