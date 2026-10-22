/*
 * A004434  Numbers that are the sum of 5 distinct nonzero squares.
 * https://oeis.org/A004434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers that are the sum of 5 distinct nonzero squares. For n <=
 *            121, find the n-th such number. For n > 121, a(n) = n + 124
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x) {
    if (x < 2) return x;
    
    seq_t s = x;
    seq_t s_next = (s + x / s) / 2;
    
    while (s_next < s) {
        s = s_next;
        s_next = (s + x / s) / 2;
    }
    
    return s;
}

static int is_sum_of_5_distinct_squares(seq_t num) {
    for (seq_t a = 1; a * a < num; a++) {
        for (seq_t b = a + 1; a*a + b*b < num; b++) {
            for (seq_t c = b + 1; a*a + b*b + c*c < num; c++) {
                for (seq_t d = c + 1; a*a + b*b + c*c + d*d < num; d++) {
                    seq_t sum = a*a + b*b + c*c + d*d;
                    seq_t remainder = num - sum;
                    seq_t e = isqrt_seq(remainder);
                    
                    if (e > d && e*e == remainder) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n > 121) {
        return (seq_t)n + 124;
    }
    
    static seq_t table[121];
    static int initialized = 0;
    
    if (!initialized) {
        int count = 0;
        seq_t num = 1;
        
        while (count < 121) {
            if (is_sum_of_5_distinct_squares(num)) {
                table[count] = num;
                count++;
            }
            num++;
        }
        initialized = 1;
    }
    
    return table[n - 1];
}
