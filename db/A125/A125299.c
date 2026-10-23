/*
 * A125299  Numbers whose names in English start with a consonant.
 * https://oeis.org/A125299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th non‑negative integer whose English
   cardinal name starts with a consonant letter.  No closed formula
   exists, so we examine successive integers and count the ones whose
   names start with a consonant. */

#include <stdint.h>
#include <stdbool.h>

static const bool _hundred_start[10] = {
    0,  /* 0 unused */
    0,  /* one   -> vowel ('o')      */
    1,  /* two   -> consonant        */
    1,  /* three -> consonant        */
    1,  /* four  -> consonant        */
    1,  /* five  -> consonant        */
    1,  /* six   -> consonant        */
    1,  /* seven -> consonant        */
    0,  /* eight -> vowel ('e')      */
    1   /* nine  -> consonant        */
};

static const bool _tens_start[10] = {
    0, 0,  /* 0 unused */
    1, 1, 1, 1, 1, 1, 0, 1  /* 20,30,40,50,60,70,80,90  */
};

static bool _consonant_start(uint64_t x)
{
    if (x == 0) return true;          /* "zero" -> 'z' consonant */
    while (x >= 1000) x /= 1000;      /* keep most significant 3‑digit block */
    if (x >= 100) {                   /* hundreds case: first word is a digit */
        unsigned d = (unsigned)(x / 100);   /* digit 1–9 */
        return _hundred_start[d];
    } else if (x >= 20) {             /* tens case: first word is a tens word */
        unsigned d = (unsigned)(x / 10);    /* digit 2–9 */
        return _tens_start[d];
    } else if (x >= 10) {             /* special 10–19 */
        switch ((unsigned)x) {
            case 10: return true;    /* ten     -> 't'   */
            case 11: return false;   /* eleven  -> 'e'   */
            case 12: return true;    /* twelve  -> 't'   */
            case 13: return true;    /* thirteen -> 't'  */
            case 14: return true;    /* fourteen -> 't'  */
            case 15: return true;    /* fifteen -> 'f'   */
            case 16: return true;    /* sixteen -> 's'   */
            case 17: return true;    /* seventeen -> 's' */
            case 18: return false;   /* eighteen -> 'e'  */
            case 19: return true;    /* nineteen -> 'n'  */
        }
    } else {                          /* 1–9 */
        unsigned d = (unsigned)x;
        return _hundred_start[d];
    }
    return false;                     /* unreachable */
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t value = 0;
    while (true) {
        if (_consonant_start(value)) {
            ++count;
            if (count == n) break;
        }
        ++value;
    }
    return (seq_t)value;
}
