/*
 * A007485  Number of letters in n (in Dutch) counting 'ij' as one letter.
 * https://oeis.org/A007485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   Number of letters in the Dutch word for n, counting 'ij' as one
 *            letter
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   build_word_0_999
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/build_word_0_999.h"

static seq_t count_letters(const char *word) {
    seq_t count = 0;
    for (size_t i = 0; word[i]; i++) {
        if (word[i] == 'i' && word[i+1] == 'j') {
            count++;
            i++;
        } else {
            count++;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    char word[256];
    
    if (n < 1000) {
        build_word_0_999((unsigned int)n, word);
    } else {
        unsigned int thousands = (unsigned int)n / 1000;
        unsigned int rest = (unsigned int)n % 1000;
        
        word[0] = '\0';
        
        if (thousands == 1) {
            strcpy(word, "duizend");
        } else {
            char thousands_word[256];
            build_word_0_999(thousands, thousands_word);
            strcpy(word, thousands_word);
            strcat(word, "duizend");
        }
        
        if (rest > 0) {
            char rest_word[256];
            build_word_0_999(rest, rest_word);
            strcat(word, rest_word);
        }
    }
    
    return count_letters(word);
}
