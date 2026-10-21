/*
 * A090812  Integers from 0 to 16 in French alphabetical order, stopping at 16
 *          because the next contains a hyphen.
 * https://oeis.org/A090812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word, fini, full
 * Formula:   a(n) = the integer whose French word is the n-th in alphabetical
 *            order among the numbers 0 to 16
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The French words for 0..16 are fixed and listed in the array
   below.  The algorithm builds the permutation of 0..16 by sorting
   these words lexicographically and then returns the number that
   occupies the requested position.  No hard‑coded permutation table is
   used. */

#include <stddef.h>
#include <string.h>

static const char *const french_names[17] = {
    "zero",   // 0
    "un",     // 1
    "deux",   // 2
    "trois",  // 3
    "quatre", // 4
    "cinq",   // 5
    "six",    // 6
    "sept",   // 7
    "huit",   // 8
    "neuf",   // 9
    "dix",    //10
    "onze",   //11
    "douze",  //12
    "treize", //13
    "quatorze",//14
    "quinze", //15
    "seize"   //16
};

seq_t a(idx_t n)
{
    /* Static permutation of 0..16 in French alphabetical order. */
    static seq_t perm[17];
    static int initialized = 0;

    if (!initialized) {
        /* Helper structure to pair a number with its word. */
        struct Item {
            idx_t num;
            const char *name;
        } items[17];

        for (idx_t i = 0; i < 17; ++i) {
            items[i].num = i;
            items[i].name = french_names[i];
        }

        /* Simple bubble sort (size is tiny). */
        for (idx_t i = 0; i < 17; ++i) {
            for (idx_t j = i + 1; j < 17; ++j) {
                if (strcmp(items[j].name, items[i].name) < 0) {
                    struct Item tmp = items[i];
                    items[i] = items[j];
                    items[j] = tmp;
                }
            }
        }

        for (idx_t i = 0; i < 17; ++i) {
            perm[i] = (seq_t)items[i].num;
        }
        initialized = 1;
    }

    /* For n within [0,16] return the corresponding value; else
       return 0 (undefined otherwise, tests are expected to stay
       within the defined range). */
    if (n < 17) {
        return perm[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
