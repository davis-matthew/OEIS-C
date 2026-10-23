/*
 * A126767  Map the sentence "What is the next number in this sequence?" to
 *          numbers by translating each letter to its position in the alphabet
 *          (a -> 1, b -> 2, ..., z -> 26) and then adding the numbers
 *          corresponding to the letters in each word.
 * https://oeis.org/A126767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb, fini, full
 * Formula:   a(n) = sum of letter values of the ((n-1) mod 8)-th word of the
 *            sentence "What is the next number in this sequence?"
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <ctype.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The 8 words of the sentence, excluding punctuation */
    static const char *const words[8] =
        {"What", "is", "the", "next", "number", "in", "this", "sequence"};

    static seq_t sums[8];
    static bool initialized = false;

    if (!initialized) {
        for (size_t i = 0; i < 8; ++i) {
            const char *w = words[i];
            seq_t s = 0;
            for (const char *p = w; *p; ++p) {
                int c = (int)*p;
                int val = 0;
                if ('A' <= c && c <= 'Z')
                    val = c - 'A' + 1;
                else if ('a' <= c && c <= 'z')
                    val = c - 'a' + 1;
                if (val)
                    s += (seq_t)val;
            }
            sums[i] = s;
        }
        initialized = true;
    }

    idx_t idx = (n - 1) % 8; /* (n-1) guarantees n>=1, offset handling */
    return sums[(size_t)idx];
}
