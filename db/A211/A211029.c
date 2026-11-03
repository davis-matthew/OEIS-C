/*
 * A211029  Triangle read by rows in which row n lists the binary words of
 *          length n over the alphabet {1,2} with no initial repeats.
 * https://oeis.org/A211029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 594, u64 n <= 8998,
 *            u128 n <= 8998, big any n
 * Validated: standard, pass, 54 terms through n = 8945 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation of A211029: enumerate binary words of length n that have no initial repeated
   block, and return the n-th such word (1‑based) interpreted in digits 1/2. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Convert a bit pattern (0/1) to a decimal value with digits 1/2. */
static inline seq_t bits_to_value(const unsigned char bits[], int len)
{
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        val = val * 10 + (bits[i] ? 2 : 1);
    }
    return val;
}

/* Return true if the prefix of length len contains an initial block repeated twice. */
static inline bool is_valid_partial(const unsigned char bits[], int len)
{
    for (int k = 1; k * 2 <= len; ++k) {
        bool same = true;
        for (int j = 0; j < k; ++j) {
            if (bits[j] != bits[k + j]) {
                same = false;
                break;
            }
        }
        if (same)
            return false;
    }
    return true;
}

/* Recursive generator that counts words and stops when the target index is reached. */
static void generate_len(int len, unsigned char *bits, int pos,
                         idx_t *curIdx, idx_t targetIdx, seq_t *targetVal,
                         bool *found)
{
    if (*found) return;
    if (pos == len) {
        if (*curIdx == targetIdx) {
            *targetVal = bits_to_value(bits, len);
            *found = true;
        }
        ++(*curIdx);
        return;
    }

    for (int bit = 0; bit < 2; ++bit) {
        bits[pos] = (unsigned char)bit;
        if (is_valid_partial(bits, pos + 1))
            generate_len(len, bits, pos + 1, curIdx, targetIdx, targetVal, found);
        if (*found) return;
    }
}

/* Generate all valid words of a given length, return the count and optionally the
   target word if it is found. */
static idx_t generate_row_and_check(int len, idx_t targetIdx, seq_t *answer,
                                    bool *found)
{
    unsigned char bits[64];    /* sufficient for the required lengths (even for n≈10 000) */
    idx_t curIdx = 0;
    *found = false;
    generate_len(len, bits, 0, &curIdx, targetIdx, answer, found);
    return curIdx;            /* total number of words of this length */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset starts at 1; undefined for n=0 */
    idx_t cumulative = 0;
    for (int len = 1; ; ++len) {
        idx_t targetInRow = (n > cumulative) ? n - cumulative - 1 : (idx_t)-1;
        seq_t ans = 0;
        bool found = false;
        idx_t wordsInRow = generate_row_and_check(len, targetInRow, &ans, &found);
        if (found)
            return ans;
        cumulative += wordsInRow;
    }
}
