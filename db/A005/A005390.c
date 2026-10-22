/*
 * A005390  Number of Hamiltonian circuits on 2n X 6 rectangle.
 * https://oeis.org/A005390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 53*a(n-1) - 802*a(n-2) + 4463*a(n-3) - 10928*a(n-4) +
 *            13708*a(n-5) - 12157*a(n-6) + 7032*a(n-7) - 11272*a(n-8) +
 *            15064*a(n-9) - 13336*a(n-10) + 5948*a(n-11) - 792*a(n-12) -
 *            96*a(n-13) - 4*a(n-14) for n >= 15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *memo = NULL;
static idx_t memo_size = 0;
static idx_t computed_up_to = 0;

static const int p[] = {1, -16, -87, 1070, -2206, 1960, -2448, 1053, 392, -1517, 1012, -120, -28, -2};
static const int q[] = {1, -53, 802, -4463, 10928, -13708, 12157, -7032, 11272, -15064, 13336, -5948, 792, 96, 4};
static const int recur[] = {53, -802, 4463, -10928, 13708, -12157, 7032, -11272, 15064, -13336, 5948, -792, -96, -4};

seq_t a(idx_t n) {
    if (memo == NULL) {
        idx_t init_size = (n > 1000) ? n + 1 : 1001;
        memo = (seq_t *)calloc(init_size, sizeof(seq_t));
        memo_size = init_size;
    } else if (n >= memo_size) {
        idx_t new_size = n + 1 + (n >> 2);
        seq_t *new_memo = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_memo) exit(1);
        memset(new_memo + memo_size, 0, (new_size - memo_size) * sizeof(seq_t));
        memo = new_memo;
        memo_size = new_size;
    }
    
    while (computed_up_to < n) {
        idx_t i = computed_up_to + 1;
        
        if (i <= 14) {
            seq_t result = (seq_t)p[i-1];
            for (int j = 1; j < (int)i; j++) {
                result -= (seq_t)q[j] * memo[i - j];
            }
            memo[i] = result;
        } else {
            seq_t result = (seq_t)0;
            for (int j = 0; j < 14; j++) {
                result += (seq_t)recur[j] * memo[i - (idx_t)(j + 1)];
            }
            memo[i] = result;
        }
        
        computed_up_to = i;
    }
    
    return memo[n];
}
