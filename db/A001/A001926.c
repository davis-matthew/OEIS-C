/*
 * A001926  G.f.: (1+x)^2/[(1-x)^4(1-x-x^2)^3].
 * https://oeis.org/A001926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   g.f. (1+x)^2/[(1-x)^4(1-x-x^2)^3]
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 26, u64 n <= 68, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    #define MEMO_SIZE 100000
    static seq_t memo[MEMO_SIZE];
    static idx_t computed_up_to = 0;
    static int memo_initialized = 0;
    
    if (!memo_initialized) {
        memo[0] = 1;
        computed_up_to = 0;
        memo_initialized = 1;
    }
    
    if (n < MEMO_SIZE) {
        while (computed_up_to < n && computed_up_to < MEMO_SIZE - 1) {
            idx_t i = computed_up_to + 1;
            seq_t result = 0;
            
            if (i >= 1) result += 7 * memo[i-1];
            if (i >= 2) result -= 18 * memo[i-2];
            if (i >= 3) result += 17 * memo[i-3];
            if (i >= 4) result += 7 * memo[i-4];
            if (i >= 5) result -= 24 * memo[i-5];
            if (i >= 6) result += 9 * memo[i-6];
            if (i >= 7) result += 9 * memo[i-7];
            if (i >= 8) result -= 6 * memo[i-8];
            if (i >= 9) result -= memo[i-9];
            if (i >= 10) result += memo[i-10];
            
            if (i == 1) result += 2;
            else if (i == 2) result += 1;
            
            memo[i] = result;
            computed_up_to = i;
        }
        return memo[n];
    }
    
    while (computed_up_to < MEMO_SIZE - 1) {
        idx_t i = computed_up_to + 1;
        seq_t result = 0;
        
        if (i >= 1) result += 7 * memo[i-1];
        if (i >= 2) result -= 18 * memo[i-2];
        if (i >= 3) result += 17 * memo[i-3];
        if (i >= 4) result += 7 * memo[i-4];
        if (i >= 5) result -= 24 * memo[i-5];
        if (i >= 6) result += 9 * memo[i-6];
        if (i >= 7) result += 9 * memo[i-7];
        if (i >= 8) result -= 6 * memo[i-8];
        if (i >= 9) result -= memo[i-9];
        if (i >= 10) result += memo[i-10];
        
        if (i == 1) result += 2;
        else if (i == 2) result += 1;
        
        memo[i] = result;
        computed_up_to = i;
    }
    
    seq_t last10[10];
    for (idx_t i = 0; i < 10; i++) {
        last10[i] = memo[MEMO_SIZE - 10 + i];
    }
    
    for (idx_t i = MEMO_SIZE; i <= n; i++) {
        seq_t result = 7 * last10[(i-1) % 10]
                     - 18 * last10[(i-2) % 10]
                     + 17 * last10[(i-3) % 10]
                     + 7 * last10[(i-4) % 10]
                     - 24 * last10[(i-5) % 10]
                     + 9 * last10[(i-6) % 10]
                     + 9 * last10[(i-7) % 10]
                     - 6 * last10[(i-8) % 10]
                     - last10[(i-9) % 10]
                     + last10[(i-10) % 10];
        
        last10[i % 10] = result;
    }
    
    return last10[n % 10];
}
