/*
 * A038842  Primes which when converted to base 36 make single letters or
 *          English words.
 * https://oeis.org/A038842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038842: primes whose base‑36 representation is a single
 * letter (A–Z) or one of the following words:
 * B D H J N T V AD AN AT CP CT CZ DB DJ DZ EH FT GP HT IB ID IT
 * MT NV OD OH ON PT SB SD TH UN UT VD WT YD AAH ACT ADD ADZ ATT
 * BAD BAH BAN BEN BOB
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* The accepted words (single letters are treated automatically). */
static const char *WORD_LIST[] = {
    "B","D","H","J","N","T","V",
    "AD","AN","AT","CP","CT","CZ","DB","DJ","DZ",
    "EH","FT","GP","HT","IB","ID","IT","MT","NV",
    "OD","OH","ON","PT","SB","SD","TH","UN","UT",
    "VD","WT","YD",
    "AAH","ACT","ADD","ADZ","ATT",
    "BAD","BAH","BAN","BEN","BOB"
};
static const size_t WORD_COUNT = sizeof(WORD_LIST) / sizeof(WORD_LIST[0]);

/* Base‑36 digit table. */
static const char DIGIT_MAP[36] = {
    '0','1','2','3','4','5','6','7','8','9',
    'A','B','C','D','E','F','G','H','I','J',
    'K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z'
};

/* Return true if val, expressed in base 36, is either a single letter
 * (A–Z) or one of the accepted words. */
static bool is_valid_word(seq_t val)
{
    if (val == (seq_t)0) return false;

    int len = 0;
    seq_t v = val;
    char rev[128];

    while (v > (seq_t)0) {
        int d = (int)(v % (seq_t)36);
        rev[len++] = DIGIT_MAP[d];
        v = v / (seq_t)36;
    }

    /* Single letter case: only digits 10–35 correspond to letters. */
    if (len == 1) {
        return val >= (seq_t)10;
    }

    /* Reconstruct the word in correct order. */
    char word[128];
    for (int i = 0; i < len; ++i) {
        word[i] = rev[len - 1 - i];
    }
    word[len] = '\0';

    /* Check against the word list. */
    for (size_t i = 0; i < WORD_COUNT; ++i) {
        if (strcmp(word, WORD_LIST[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* Return the nth term of the sequence (n>=1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* offset is 1; undefined for n=0, but return 0 to avoid UB. */
        return (seq_t)0;
    }

    idx_t found = 0;

    /* Handle the even prime 2 separately. */
    if (is_prime_big((seq_t)2) && is_valid_word((seq_t)2)) {
        ++found;
        if (found == n) {
            return (seq_t)2;
        }
    }

    seq_t candidate = 3;   /* start with the first odd prime candidate */

    while (found < n) {
        if (is_prime_big(candidate) && is_valid_word(candidate)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        candidate += 2;    /* skip even numbers */
    }

    /* Unreachable, but keeps compiler happy. */
    return (seq_t)0;
}
