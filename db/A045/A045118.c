/*
 * A045118  Numbers whose base-4 representation contains exactly one 1 and
 *          four 3's.
 * https://oeis.org/A045118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = n-th integer whose base‑4 representation
   contains exactly one digit 1 and exactly four digits 3. */

#include <stdint.h>

/* Compute nCk for small n, using seq_t arithmetic. */
static seq_t comb_seq_t(unsigned int n, unsigned int k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (unsigned int i = 1; i <= k; ++i)
        r = r * (n - i + 1) / i;
    return r;
}

/* Compute 2^exp for unsigned exp. */
static seq_t pow2_seq_t(unsigned int exp)
{
    seq_t r = 1;
    while (exp--) r *= 2;
    return r;
}

/* Number of length‑R sequences of digits {0,1,2,3} that
   contain exactly remOne 1's and remThrees 3's.
   (Digits 0 and 2 are free in the remaining positions.) */
static seq_t count_remaining(unsigned int remOne, unsigned int remThrees,
                             unsigned int R)
{
    if (remOne > R || remThrees > R || remOne + remThrees > R)
        return 0;
    seq_t c1 = comb_seq_t(R, remOne);
    seq_t c2 = comb_seq_t(R - remOne, remThrees);
    seq_t p  = pow2_seq_t(R - remOne - remThrees);
    return c1 * c2 * p;
}

/* Total number of base‑4 numbers of exact length L
   that contain exactly one '1' and exactly four '3's. */
static seq_t countLen(unsigned int L)
{
    if (L < 5) return 0;
    unsigned int R = L - 1;            /* positions after the leading digit */
    seq_t total = 0;

    /* Leading digit 1 */
    if (R >= 4) {
        seq_t c = comb_seq_t(R, 4) * pow2_seq_t(R - 4);
        total += c;
    }

    /* Leading digit 3 */
    if (R >= 4) {
        seq_t c = comb_seq_t(R, 1) * comb_seq_t(R - 1, 3)
                  * pow2_seq_t(R - 4);
        total += c;
    }

    /* Leading digit 2 */
    if (R >= 5) {
        seq_t c = comb_seq_t(R, 1) * comb_seq_t(R - 1, 4)
                  * pow2_seq_t(R - 5);
        total += c;
    }

    return total;
}

/* Unranking algorithm: construct the n‑th number directly. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, behaviour for n=0 arbitrary */

    /* Find the length that contains the n‑th term. */
    seq_t cumulative = 0;
    unsigned int targetLen = 0;
    seq_t rankWithin = 0;

    for (unsigned int L = 5; ; ++L) {
        seq_t cnt = countLen(L);
        if (cumulative + cnt >= (seq_t)n) {
            targetLen = L;
            rankWithin = (seq_t)n - cumulative - 1;   /* zero‑based rank */
            break;
        }
        cumulative += cnt;
    }

    /* Unrank: build the number digit by digit. */
    seq_t value = 0;
    unsigned int remOne = 1;
    unsigned int remThrees = 4;

    for (unsigned int pos = 0; pos < targetLen; ++pos) {
        unsigned int R = targetLen - pos - 1;   /* remaining positions */

        /* Try digits in increasing order: 0,1,2,3. */
        for (int d = 0; d <= 3; ++d) {
            /* No leading zero. */
            if (pos == 0 && d == 0) continue;

            /* Check whether this digit is permissible. */
            unsigned int nRemOne = remOne;
            unsigned int nRemThrees = remThrees;
            if (d == 1) {
                if (nRemOne == 0) continue;
                nRemOne--;
            } else if (d == 3) {
                if (nRemThrees == 0) continue;
                nRemThrees--;
            }

            seq_t cnt = count_remaining(nRemOne, nRemThrees, R);
            if (rankWithin < cnt) {
                /* Choose this digit. */
                value = value * 4 + d;
                remOne = nRemOne;
                remThrees = nRemThrees;
                break;
            } else {
                rankWithin -= cnt;
            }
        }
    }
    return value;
}
