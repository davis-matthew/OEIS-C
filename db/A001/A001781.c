/*
 * A001781  Expansion of 1/((1+x)*(1-x)^10).
 * https://oeis.org/A001781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 35*a(n-2) + 75*a(n-3) - 90*a(n-4) + 42*a(n-5)
 *            + 42*a(n-6) - 90*a(n-7) + 75*a(n-8) - 35*a(n-9) + 9*a(n-10) -
 *            a(n-11)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 47, u64 n <= 613,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

#define MAX_MEMO 10001

static seq_t memo[MAX_MEMO];
static idx_t memo_max = 10;
static int memo_initialized = 0;

seq_t a(idx_t n) {
    if (n < 11) {
        static seq_t base_cases[11] = {
            1, 9, 46, 174, 541, 1461, 3544, 7896, 16414, 32206, 60172
        };
        return base_cases[n];
    }
    
    if (n >= MAX_MEMO) {
        static seq_t base_cases[11] = {
            1, 9, 46, 174, 541, 1461, 3544, 7896, 16414, 32206, 60172
        };
        
        seq_t buffer[11];
        for (int i = 0; i < 11; i++) {
            buffer[i] = base_cases[i];
        }
        
        for (idx_t i = 11; i <= n; i++) {
            seq_t next = (seq_t)9 * buffer[10] 
                       - (seq_t)35 * buffer[9] 
                       + (seq_t)75 * buffer[8] 
                       - (seq_t)90 * buffer[7] 
                       + (seq_t)42 * buffer[6] 
                       + (seq_t)42 * buffer[5] 
                       - (seq_t)90 * buffer[4] 
                       + (seq_t)75 * buffer[3] 
                       - (seq_t)35 * buffer[2] 
                       + (seq_t)9 * buffer[1] 
                       - buffer[0];
            
            for (int j = 0; j < 10; j++) {
                buffer[j] = buffer[j+1];
            }
            buffer[10] = next;
        }
        
        return buffer[10];
    }
    
    if (!memo_initialized) {
        static seq_t base_cases[11] = {
            1, 9, 46, 174, 541, 1461, 3544, 7896, 16414, 32206, 60172
        };
        for (int i = 0; i < 11; i++) {
            memo[i] = base_cases[i];
        }
        memo_initialized = 1;
    }
    
    while (memo_max < n) {
        idx_t i = memo_max + 1;
        seq_t val = (seq_t)9 * memo[i-1] 
                  - (seq_t)35 * memo[i-2] 
                  + (seq_t)75 * memo[i-3] 
                  - (seq_t)90 * memo[i-4] 
                  + (seq_t)42 * memo[i-5] 
                  + (seq_t)42 * memo[i-6] 
                  - (seq_t)90 * memo[i-7] 
                  + (seq_t)75 * memo[i-8] 
                  - (seq_t)35 * memo[i-9] 
                  + (seq_t)9 * memo[i-10] 
                  - memo[i-11];
        memo[i] = val;
        memo_max = i;
    }
    
    return memo[n];
}
