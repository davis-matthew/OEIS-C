/*
 * A006944  Number of letters in the n-th ordinal number (in American
 *          English).
 * https://oeis.org/A006944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, nice, easy
 * Formula:   Count the letters in the English ordinal representation of n,
 *            excluding hyphens and spaces
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cardinal_letters
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cardinal_letters.h"
static seq_t cardinal_letters(idx_t x);

seq_t a(idx_t n) {
    static const uint8_t base_ordinals[] = {
        5,  // 1: first
        6,  // 2: second
        5,  // 3: third
        6,  // 4: fourth
        5,  // 5: fifth
        5,  // 6: sixth
        7,  // 7: seventh
        6,  // 8: eighth
        5,  // 9: ninth
        5,  // 10: tenth
        8,  // 11: eleventh
        7,  // 12: twelfth
        10, // 13: thirteenth
        10, // 14: fourteenth
        9,  // 15: fifteenth
        9,  // 16: sixteenth
        11, // 17: seventeenth
        10, // 18: eighteenth
        10, // 19: nineteenth
        9   // 20: twentieth
    };
    
    static const uint8_t tens_ordinals[] = {
        9,  // 20: twentieth
        9,  // 30: thirtieth
        8,  // 40: fortieth
        8,  // 50: fiftieth
        8,  // 60: sixtieth
        10, // 70: seventieth
        9,  // 80: eightieth
        9   // 90: ninetieth
    };
    
    static const uint8_t tens_cardinals[] = {
        6, // 20: twenty
        6, // 30: thirty
        5, // 40: forty
        5, // 50: fifty
        5, // 60: sixty
        7, // 70: seventy
        6, // 80: eighty
        6  // 90: ninety
    };
    
    static const uint8_t units_ordinals[] = {
        5, // 1: first
        6, // 2: second
        5, // 3: third
        6, // 4: fourth
        5, // 5: fifth
        5, // 6: sixth
        7, // 7: seventh
        6, // 8: eighth
        5  // 9: ninth
    };
    
    static const uint8_t units_cardinals[] = {
        3, // 1: one
        3, // 2: two
        5, // 3: three
        4, // 4: four
        4, // 5: five
        3, // 6: six
        5, // 7: seven
        5, // 8: eight
        4  // 9: nine
    };
    
    if (n <= 20) {
        return (seq_t)base_ordinals[n - 1];
    } else if (n < 100) {
        idx_t tens = n / 10;
        idx_t ones = n % 10;
        if (ones == 0) {
            return (seq_t)tens_ordinals[tens - 2];
        } else {
            return (seq_t)tens_cardinals[tens - 2] + (seq_t)units_ordinals[ones - 1];
        }
    } else if (n < 1000) {
        idx_t hundreds = n / 100;
        idx_t remainder = n % 100;
        if (remainder == 0) {
            return (seq_t)units_cardinals[hundreds - 1] + (seq_t)9;
        } else {
            return (seq_t)units_cardinals[hundreds - 1] + (seq_t)7 + a(remainder);
        }
    } else if (n < 1000000) {
        idx_t thousands = n / 1000;
        idx_t remainder = n % 1000;
        seq_t thousands_part = cardinal_letters(thousands);
        if (remainder == 0) {
            return thousands_part + (seq_t)10;
        } else {
            return thousands_part + (seq_t)8 + a(remainder);
        }
    }
    
    return (seq_t)0;
}
