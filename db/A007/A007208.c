/*
 * A007208  Number of letters in German name of n.
 * https://oeis.org/A007208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   Length of German name of n, counting letters only
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t len_0_99(idx_t m) {
    const seq_t lens_0_9_standalone[] = {4, 4, 4, 4, 4, 4, 5, 6, 4, 4};
    const seq_t lens_0_9_compound[] = {0, 3, 4, 4, 4, 4, 5, 6, 4, 4};
    const seq_t lens_10_19[] = {4, 3, 5, 8, 8, 8, 8, 8, 8, 8};
    const seq_t lens_tens[] = {0, 0, 7, 7, 7, 7, 7, 7, 7, 7};
    
    if (m < 10) {
        return lens_0_9_standalone[m];
    } else if (m < 20) {
        return lens_10_19[m - 10];
    } else {
        seq_t len = lens_tens[m / 10];
        if (m % 10 > 0) {
            len += 3;
            len += lens_0_9_compound[m % 10];
        }
        return len;
    }
}

static seq_t len_0_999(idx_t m) {
    const seq_t lens_0_9_compound[] = {0, 3, 4, 4, 4, 4, 5, 6, 4, 4};
    
    if (m < 100) {
        return len_0_99(m);
    } else {
        seq_t len = 0;
        idx_t hundreds = m / 100;
        idx_t remainder = m % 100;
        
        if (hundreds > 1) {
            len += lens_0_9_compound[hundreds];
        }
        len += 7;
        
        if (remainder > 0) {
            len += len_0_99(remainder);
        }
        
        return len;
    }
}

seq_t a(idx_t n) {
    if (n < 1000) {
        return len_0_999(n);
    } else if (n < 1000000) {
        seq_t len = 0;
        idx_t thousands = n / 1000;
        idx_t remainder = n % 1000;
        
        if (thousands > 1) {
            len += len_0_999(thousands);
        }
        len += 7;
        
        if (remainder > 0) {
            len += len_0_999(remainder);
        }
        
        return len;
    } else if (n < 1000000000) {
        seq_t len = 0;
        idx_t millions = n / 1000000;
        idx_t remainder = n % 1000000;
        
        if (millions == 1) {
            len = 4 + 7;
        } else {
            len = len_0_999(millions) + 9;
        }
        
        if (remainder > 0) {
            len += a(remainder);
        }
        
        return len;
    } else if (n < 1000000000000ULL) {
        seq_t len = 0;
        idx_t billions = n / 1000000000;
        idx_t remainder = n % 1000000000;
        
        if (billions == 1) {
            len = 4 + 9;
        } else {
            len = len_0_999(billions) + 10;
        }
        
        if (remainder > 0) {
            len += a(remainder);
        }
        
        return len;
    } else if (n < 1000000000000000ULL) {
        seq_t len = 0;
        idx_t trillions = n / 1000000000000ULL;
        idx_t remainder = n % 1000000000000ULL;
        
        if (trillions == 1) {
            len = 4 + 7;
        } else {
            len = len_0_999(trillions) + 9;
        }
        
        if (remainder > 0) {
            len += a(remainder);
        }
        
        return len;
    } else if (n < 1000000000000000000ULL) {
        seq_t len = 0;
        idx_t billiards = n / 1000000000000000ULL;
        idx_t remainder = n % 1000000000000000ULL;
        
        if (billiards == 1) {
            len = 4 + 9;
        } else {
            len = len_0_999(billiards) + 10;
        }
        
        if (remainder > 0) {
            len += a(remainder);
        }
        
        return len;
    } else {
        seq_t len = 0;
        idx_t trillionCount = n / 1000000000000000000ULL;
        idx_t remainder = n % 1000000000000000000ULL;
        
        if (trillionCount == 1) {
            len = 4 + 8;
        } else {
            len = len_0_999(trillionCount) + 10;
        }
        
        if (remainder > 0) {
            len += a(remainder);
        }
        
        return len;
    }
}
