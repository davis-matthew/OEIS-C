/*
 * A005589  Number of letters in the US English name of n, excluding spaces
 *          and hyphens.
 * https://oeis.org/A005589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, nice
 * Formula:   count letters in English name of n (no spaces, no hyphens)
 * Range:     u8 n <= 9302, u16 n <= 9302, u32 n <= 9302, u64 n <= 9302,
 *            u128 n <= 9302, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const uint8_t ones_letters[] = {
        4,  // zero
        3,  // one
        3,  // two
        5,  // three
        4,  // four
        4,  // five
        3,  // six
        5,  // seven
        5,  // eight
        4   // nine
    };
    
    static const uint8_t teens_letters[] = {
        3,  // ten
        6,  // eleven
        6,  // twelve
        8,  // thirteen
        8,  // fourteen
        7,  // fifteen
        7,  // sixteen
        9,  // seventeen
        8,  // eighteen
        8   // nineteen
    };
    
    static const uint8_t tens_letters[] = {
        0,  // (unused)
        0,  // (unused)
        6,  // twenty
        6,  // thirty
        5,  // forty
        5,  // fifty
        5,  // sixty
        7,  // seventy
        6,  // eighty
        6   // ninety
    };
    
    static const uint8_t scale_letters[] = {
        0,  // ones (no scale word)
        8,  // thousand
        7,  // million
        7,  // billion
        8,  // trillion
        11, // quadrillion
        11  // quintillion
    };
    
    if (n == 0) {
        return 4;
    }
    
    seq_t count = 0;
    int scale = 0;
    
    while (n > 0) {
        uint32_t group = (uint32_t)(n % 1000);
        
        if (group > 0) {
            if (group >= 100) {
                uint32_t hundreds = group / 100;
                count += ones_letters[hundreds];
                count += 7;
                group %= 100;
            }
            
            if (group >= 20) {
                uint32_t tens = group / 10;
                count += tens_letters[tens];
                uint32_t ones = group % 10;
                if (ones > 0) {
                    count += ones_letters[ones];
                }
            } else if (group >= 10) {
                count += teens_letters[group - 10];
            } else if (group > 0) {
                count += ones_letters[group];
            }
            
            if (scale > 0 && scale < 7) {
                count += scale_letters[scale];
            }
        }
        
        n /= 1000;
        scale++;
    }
    
    return count;
}
