/*
 * A007861  Mahonian statistics on S_n which split (a(n)=n!.a(n-1)^n).
 * https://oeis.org/A007861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * a(n-1)^n
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_N 10100

static seq_t memo[MAX_N];
static int computed[MAX_N];

seq_t a(idx_t n) {
    if (n < 1 || n >= MAX_N) return 0;
    
    if (computed[n]) {
        return memo[n];
    }
    
    if (n == 1) {
        memo[1] = 1;
        computed[1] = 1;
        return 1;
    }
    
    idx_t start = 1;
    seq_t prev = 0;
    
    for (idx_t i = n - 1; i >= 1; i--) {
        if (computed[i]) {
            start = i + 1;
            prev = memo[i];
            break;
        }
    }
    
    if (start == 1) {
        if (!computed[1]) {
            memo[1] = 1;
            computed[1] = 1;
        }
        prev = memo[1];
        start = 2;
    }
    
    for (idx_t i = start; i <= n; i++) {
        seq_t fact = 1;
        for (idx_t j = 2; j <= i; j++) {
            fact = fact * (seq_t)j;
        }
        
        seq_t power_val = 1;
        seq_t b = prev;
        idx_t exp = i;
        while (exp > 0) {
            if (exp & 1) {
                power_val = power_val * b;
            }
            b = b * b;
            exp >>= 1;
        }
        
        prev = fact * power_val;
        memo[i] = prev;
        computed[i] = 1;
    }
    
    return memo[n];
}
