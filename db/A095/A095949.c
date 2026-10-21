/*
 * A095949  Position of consonants in English alphabet.
 * https://oeis.org/A095949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = position of the n-th consonant in the alphabet, the
 *            sequence is periodic with period 22 (the 22 consonants)
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t consonants[22];
static bool consonants_initialized = false;

seq_t a(idx_t n)
{
    /* Initialise the consonant list once – the letters excluded are
       A(1), I(9), O(15) and U(21). */
    if (!consonants_initialized) {
        idx_t pos = 0;
        for (idx_t i = 1; i <= 26; ++i) {
            if (i == 1 || i == 9 || i == 15 || i == 21)
                continue;                 /* vowel */
            consonants[pos++] = (seq_t)i;  /* consonant position */
        }
        consonants_initialized = true;
    }

    /* Periodic index: 0 … 21 */
    idx_t p = (n - 1) % 22;
    return consonants[p];
}
