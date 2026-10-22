/*
 * A001278  Number of permutations of length n by rises.
 * https://oeis.org/A001278
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*a(n-1) - 3*(n-2)*a(n-2) - 2*(n-1)*a(n-3) +
 *            (6*n-17)*a(n-4) + (n-5)*a(n-5) - 3*(n-4)*a(n-6)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base cases: a(0..3)=0, a(4)=1, a(5)=11 */

#define MEMO_SIZE 10001

static seq_t memo[MEMO_SIZE];
static int initialized = 0;

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    if (!initialized) {
        memo[4] = 1;
        memo[5] = 11;
        for (idx_t i = 6; i < MEMO_SIZE; i++) {
            seq_t a1 = memo[i - 1];
            seq_t a2 = memo[i - 2];
            seq_t a3 = memo[i - 3];
            seq_t a4 = memo[i - 4];
            seq_t a5 = memo[i - 5];
            seq_t a6 = memo[i - 6];
            
            memo[i] = (seq_t)(i + 3) * a1
                      - (seq_t)3 * (seq_t)(i - 2) * a2
                      - (seq_t)2 * (seq_t)(i - 1) * a3
                      + (seq_t)(6 * i - 17) * a4
                      + (seq_t)(i - 5) * a5
                      - (seq_t)3 * (seq_t)(i - 4) * a6;
        }
        initialized = 1;
    }
    
    if (n < MEMO_SIZE) {
        return memo[n];
    }
    
    seq_t a6 = memo[MEMO_SIZE - 6];
    seq_t a5 = memo[MEMO_SIZE - 5];
    seq_t a4 = memo[MEMO_SIZE - 4];
    seq_t a3 = memo[MEMO_SIZE - 3];
    seq_t a2 = memo[MEMO_SIZE - 2];
    seq_t a1 = memo[MEMO_SIZE - 1];
    
    for (idx_t i = MEMO_SIZE; i < n; i++) {
        seq_t anew = (seq_t)(i + 3) * a1
                     - (seq_t)3 * (seq_t)(i - 2) * a2
                     - (seq_t)2 * (seq_t)(i - 1) * a3
                     + (seq_t)(6 * i - 17) * a4
                     + (seq_t)(i - 5) * a5
                     - (seq_t)3 * (seq_t)(i - 4) * a6;
        
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = anew;
    }
    
    return a1;
}
