/*
 * A045052  Numbers whose base-4 representation contains exactly three 0's and
 *          no 2's.
 * https://oeis.org/A045052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: the n‑th integer whose base‑4 representation contains exactly
   three 0’s and no 2’s (offset 1). */

#include <stdint.h>
#include <stdbool.h>

/* Count the number of ways to fill the suffix starting at position 'pos'
   (0‑based) of a number of total length 'L', given that exactly
   'zeros_remaining' zeros must appear among the remaining digits.
   The digits allowed are {0,1,3} except the most significant digit
   (pos=0) must not be 0.  This helper never treats the most significant
   digit; it is called only for pos > 0.  The result is returned as a
   seq_t, which can be larger than 64 bits for very large lengths. */
static inline seq_t
count_completions(unsigned int L, unsigned int pos, unsigned int zeros_remaining)
{
    unsigned int remaining = L - pos;
    if (zeros_remaining > remaining) return (seq_t)0;

    /* compute binomial C(remaining, zeros_remaining) */
    seq_t comb = 1;
    unsigned int k = zeros_remaining;
    if (k > remaining - k) k = remaining - k;
    for (unsigned int i = 1; i <= k; ++i) {
        comb *= (unsigned int)(remaining - k + i);
        comb /= i;
    }

    unsigned int exp = remaining - zeros_remaining;
    seq_t pow2 = ((seq_t)1 << exp);

    return comb * pow2;
}

/* Number of valid numbers of length L (in base‑4).  The first digit can
   be 1 or 3, the remaining L-1 digits are from {0,1,3} with exactly
   three zeros in those positions. */
static inline seq_t
count_level(unsigned int L)
{
    if (L < 4) return (seq_t)0;
    /* 2 choices for the leading digit (1 or 3). */
    return (seq_t)2 * count_completions(L, 1, 3);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 – guard */

    idx_t n64 = (idx_t)n;
    idx_t cumulative = 0;
    unsigned int L = 4;

    /* find the length L such that the n-th number lies within this
       block of numbers of length L. */
    while (true) {
        seq_t cnt = count_level(L);
        /* if cumulative + cnt overflows idx_t we are already past n */
        if (cnt == (seq_t)0 || cumulative + (idx_t)cnt >= n64) break;
        cumulative += (idx_t)cnt;
        ++L;
    }

    idx_t rank = n64 - cumulative - 1;          /* 0‑based rank within length L */
    seq_t value = 0;
    int zeros_remaining = 3;

    for (unsigned int pos = 0; pos < L; ++pos) {
        if (pos == 0) {
            /* most significant digit: 1 then 3 */
            const int cand[2] = {1, 3};
            for (int k = 0; k < 2; ++k) {
                seq_t cnt = count_completions(L, pos + 1, zeros_remaining);
                seq_t rank_seq = (seq_t)rank;
                if (rank_seq < cnt) {
                    value = (value << 2) | (seq_t)cand[k];
                    break;
                } else {
                    rank -= (idx_t)cnt;
                }
            }
        } else {
            /* other positions: try 0, then 1, then 3 (ascending order) */
            const int cand[3] = {0, 1, 3};
            for (int k = 0; k < 3; ++k) {
                int d = cand[k];
                if (d == 0) {
                    if (zeros_remaining == 0) continue;
                    seq_t cnt = count_completions(L, pos + 1, zeros_remaining - 1);
                    seq_t rank_seq = (seq_t)rank;
                    if (rank_seq < cnt) {
                        value = (value << 2) | (seq_t)0;
                        zeros_remaining -= 1;
                        break;
                    } else {
                        rank -= (idx_t)cnt;
                    }
                } else {
                    seq_t cnt = count_completions(L, pos + 1, zeros_remaining);
                    seq_t rank_seq = (seq_t)rank;
                    if (rank_seq < cnt) {
                        value = (value << 2) | (seq_t)d;
                        break;
                    } else {
                        rank -= (idx_t)cnt;
                    }
                }
            }
        }
    }

    return value;
}
