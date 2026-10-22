/*
 * A002529  a(n) = A002527(n+1) - A002527(n) - A002526(n).
 * https://oeis.org/A002529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 10*a(n-4) + 8*a(n-5) - 2*a(n-6) -
 *            16*a(n-7) - 10*a(n-8) - 2*a(n-9) + 4*a(n-10) + 2*a(n-11) +
 *            2*a(n-13) + a(n-14)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t init[14] = {0, 0, 2, 6, 18, 46, 146, 460, 1436, 4352, 13252, 40532, 124396, 381140};
static seq_t *win = NULL;
static idx_t base = 0;

seq_t a(idx_t n) {
    if (n < 14) return init[n];
    
    if (!win) {
        win = (seq_t *)malloc(14 * sizeof(seq_t));
        for (int i = 0; i < 14; i++) win[i] = init[i];
    }
    
    while (base + 13 < n) {
        seq_t next = (seq_t)2 * win[13] + (seq_t)2 * win[12]
                   + (seq_t)10 * win[10] + (seq_t)8 * win[9]
                   - (seq_t)2 * win[8] - (seq_t)16 * win[7]
                   - (seq_t)10 * win[6] - (seq_t)2 * win[5]
                   + (seq_t)4 * win[4] + (seq_t)2 * win[3]
                   + (seq_t)2 * win[1] + win[0];
        
        for (int i = 0; i < 13; i++) win[i] = win[i + 1];
        win[13] = next;
        base++;
    }
    
    return win[n - base];
}
