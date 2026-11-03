/*
 * A001162  Numbers containing an odd number of letters.
 * https://oeis.org/A001162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = n-th positive integer whose English word representation
 *            contains an odd number of letters
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int letter_count(seq_t num) {
    static const int ones_letters[] = {
        0, 3, 3, 5, 4, 4, 3, 5, 5, 4,
        3, 6, 6, 8, 8, 7, 7, 9, 8, 8
    };
    static const int tens_letters[] = {
        0, 0, 6, 6, 5, 5, 5, 7, 6, 6
    };
    static const int scale_letters[] = {0, 8, 7, 7, 8, 11, 11, 10, 10, 9, 9, 11, 12};
    
    if (num == 0) return 4;
    
    int count = 0;
    int scale_idx = 0;
    seq_t temp = num;
    
    while (temp > 0) {
        seq_t group = temp % 1000;
        temp /= 1000;
        
        if (group == 0) {
            scale_idx++;
            continue;
        }
        
        seq_t hundreds = group / 100;
        if (hundreds > 0) {
            count += ones_letters[(int)hundreds] + 7;
        }
        
        seq_t remainder = group % 100;
        if (remainder >= 20) {
            int ten = (int)(remainder / 10);
            int one = (int)(remainder % 10);
            count += tens_letters[ten];
            if (one > 0) count += ones_letters[one];
        } else if (remainder > 0) {
            count += ones_letters[(int)remainder];
        }
        
        if (scale_idx > 0 && scale_idx < 13) {
            count += scale_letters[scale_idx];
        }
        
        scale_idx++;
    }
    
    return count;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < (seq_t)n) {
        num++;
        if (letter_count(num) % 2 == 1) {
            count++;
        }
    }
    
    return num;
}
