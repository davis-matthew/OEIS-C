/*
 * A094672  Values of n such that one or more ending terms of the
 *          lexicographically earliest squarefree ternary word (LMSFTW) of
 *          length n differ(s) from the first n terms of the LMSFTW of length
 *          n+1.
 * https://oeis.org/A094672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th position where the lexicographically earliest
 *            squarefree ternary word of length n differs from the prefix of
 *            the lexicographically earliest word of length n+1. The sequence
 *            is computed by incrementally generating the minimal words and
 *            recording those positions where a change occurs
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_square.h"

seq_t a(idx_t n)
{
    /* static state across calls */
    static idx_t curLen = 0;          /* current length of built word  */
    static idx_t pos = 0;             /* current depth during construction   */
    static uint8_t *word = NULL;      /* word digits (0,1,2)                 */
    static uint8_t *next_digit = NULL;/* next digit to try at each position   */
    static size_t w_cap = 0;          /* capacity of word arrays              */

    static seq_t *terms = NULL;       /* stored terms of the sequence        */
    static size_t terms_len = 0;      /* number of terms stored              */
    static size_t terms_cap = 0;      /* capacity of terms array             */

    static uint8_t *prevWord = NULL;  /* temporary buffer for previous word  */
    static size_t prev_cap = 0;       /* capacity of prevWord                */

    /* ---------- initialization of first word (length 1) ---------- */
    if (curLen == 0) {
        size_t init_cap = 8;
        word = (uint8_t *)malloc(init_cap);
        next_digit = (uint8_t *)malloc(init_cap);
        w_cap = init_cap;
        /* build lexicographically earliest word of length 1 */
        idx_t targetLen = 1;
        pos = 0;
        next_digit[0] = 0;
        for (;;) {
            if (pos == targetLen) break;
            while (next_digit[pos] < 3) {
                uint8_t cand = next_digit[pos];
                word[pos] = cand;
                if (has_square(word, pos)) {
                    next_digit[pos]++; /* reset for next time */
                    pos++;
                    if (pos < targetLen) next_digit[pos] = 0;
                    break;
                } else {
                    next_digit[pos]++;
                }
            }
            if (next_digit[pos] == 3) {
                next_digit[pos] = 0;
                if (pos == 0) break;          /* should never happen */
                pos--;
            }
        }
        curLen = 1;
        pos = 1; /* ready for next length */
    }

    /* ---------- generate words until we have enough terms ---------- */
    while (terms_len < n) {
        /* copy current word into prevWord for later comparison */
        if (prev_cap < w_cap) {
            free(prevWord);
            prevWord = (uint8_t *)malloc(w_cap);
            prev_cap = w_cap;
        }
        memcpy(prevWord, word, curLen);

        /* build next lexicographically earliest word of length curLen+1 */
        idx_t targetLen = curLen + 1;
        if (targetLen > w_cap) {
            size_t new_cap = w_cap * 2;
            if (new_cap < targetLen) new_cap = targetLen;
            word = (uint8_t *)realloc(word, new_cap);
            next_digit = (uint8_t *)realloc(next_digit, new_cap);
            w_cap = new_cap;
        }
        /* initialize the new position with the smallest digit to try */
        next_digit[targetLen - 1] = 0;   /* next_digit array already valid up to curLen */

        /* run DFS style construction */
        for (;;) {
            if (pos == targetLen) break;
            while (next_digit[pos] < 3) {
                uint8_t cand = next_digit[pos];
                word[pos] = cand;
                if (has_square(word, pos)) {
                    next_digit[pos]++; /* to skip the used digit next time */
                    pos++;
                    if (pos < targetLen) next_digit[pos] = 0;
                    break;
                } else {
                    next_digit[pos]++;
                }
            }
            if (next_digit[pos] == 3) {
                next_digit[pos] = 0;
                if (pos == 0) break; /* should not happen */
                pos--;
            }
        }

        /* compare new word with previous word */
        bool diff = false;
        for (idx_t i = 0; i < curLen; ++i) {
            if (word[i] != prevWord[i]) { diff = true; break; }
        }
        if (diff) {
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                if (new_cap < terms_len + 1) new_cap = terms_len + 1;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)curLen;
        }

        curLen = targetLen;
        pos = curLen;   /* ready for following length */
    }

    return terms[n - 1];
}
