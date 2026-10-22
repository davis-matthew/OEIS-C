/*
 * A002473  7-smooth numbers: positive numbers whose prime divisors are all <=
 *          7.
 * https://oeis.org/A002473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   7-smooth numbers = {2^a * 3^b * 5^c * 7^d : a,b,c,d >= 0},
 *            ordered
 * Range:     u8 n <= 75, u16 n <= 612, u32 n <= 6623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *smooth = NULL;
static idx_t smooth_cap = 0;
static idx_t smooth_len = 0;
static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (smooth == NULL) {
        smooth_cap = 1000;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        smooth[0] = 1;
        smooth_len = 1;
    }
    
    while (smooth_len < n) {
        seq_t next2 = smooth[i2] * (seq_t)2;
        seq_t next3 = smooth[i3] * (seq_t)3;
        seq_t next5 = smooth[i5] * (seq_t)5;
        seq_t next7 = smooth[i7] * (seq_t)7;
        
        seq_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;
        
        if (smooth_len >= smooth_cap) {
            smooth_cap *= 2;
            seq_t *new_smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
            memcpy(new_smooth, smooth, smooth_len * sizeof(seq_t));
            free(smooth);
            smooth = new_smooth;
        }
        
        smooth[smooth_len++] = next;
        
        if (next == next2) i2++;
        if (next == next3) i3++;
        if (next == next5) i5++;
        if (next == next7) i7++;
    }
    
    return smooth[n - 1];
}
