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
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

static void build_word_0_999(unsigned int n, char *word) {
    const char *ones[] = {
        "", "een", "twee", "drie", "vier", "vijf", "zes", "zeven", "acht", "negen",
        "tien", "elf", "twaalf", "dertien", "veertien", "vijftien", "zestien", "zeventien", "achttien", "negentien"
    };
    
    const char *tens[] = {
        "", "", "twintig", "dertig", "veertig", "vijftig", "zestig", "zeventig", "tachtig", "negentig"
    };
    
    word[0] = '\0';
    
    if (n < 20) {
        strcpy(word, ones[n]);
    } else if (n < 100) {
        int tens_digit = n / 10;
        int ones_digit = n % 10;
        
        if (ones_digit == 0) {
            strcpy(word, tens[tens_digit]);
        } else {
            strcpy(word, ones[ones_digit]);
            strcat(word, "en");
            strcat(word, tens[tens_digit]);
        }
    } else {
        int hundreds_digit = n / 100;
        int rest = n % 100;
        
        strcpy(word, ones[hundreds_digit]);
        strcat(word, "honderd");
        
        if (rest > 0) {
            if (rest < 20) {
                strcat(word, ones[rest]);
            } else {
                int tens_digit = rest / 10;
                int ones_digit = rest % 10;
                
                if (ones_digit == 0) {
                    strcat(word, tens[tens_digit]);
                } else {
                    strcat(word, ones[ones_digit]);
                    strcat(word, "en");
                    strcat(word, tens[tens_digit]);
                }
            }
        }
    }
}

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
