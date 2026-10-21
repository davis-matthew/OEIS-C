/*
 * A074322  0 if pronounced name of n-th letter of English alphabet begin with
 *          a vowel sound, otherwise 1. Different from A054638.
 * https://oeis.org/A074322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = 0 if the spoken name of the nth letter of the English
 *            alphabet starts with a vowel sound, otherwise 1; the sequence
 *            repeats every 26
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Mapping of the 0‑based alphabet index to 0/1 as defined above. */
static const uint8_t vowel_map[26] = {
    0, /* A – vowel sound */
    1, /* B – consonant */
    1, /* C – consonant */
    1, /* D – consonant */
    0, /* E – vowel sound */
    0, /* F – vowel sound ("eff") */
    1, /* G – consonant */
    0, /* H – vowel sound ("aitch") */
    0, /* I – vowel sound */
    1, /* J – consonant */
    1, /* K – consonant */
    0, /* L – vowel sound ("el") */
    0, /* M – vowel sound ("em") */
    0, /* N – vowel sound ("en") */
    0, /* O – vowel sound */
    1, /* P – consonant */
    1, /* Q – consonant */
    0, /* R – vowel sound ("ar") */
    0, /* S – vowel sound ("es") */
    1, /* T – consonant */
    1, /* U – consonant sound (starts with /j/ in "you") */
    1, /* V – consonant */
    1, /* W – consonant */
    0, /* X – vowel sound ("ex") */
    1, /* Y – consonant ("why") */
    1  /* Z – consonant */
};

seq_t a(idx_t n) {
    /* n ≥ 1; compute 0‑based index modulo 26. */
    idx_t idx = (n - 1) % 26U;
    /* Access the table; cast idx to size_t to satisfy the array index type. */
    return (seq_t)vowel_map[(size_t)idx];
}
