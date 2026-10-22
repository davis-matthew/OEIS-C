/*
 * A003997  Sums of distinct positive cubes.
 * https://oeis.org/A003997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming to enumerate sums of distinct positive cubes; for n > 9970, a(n) = n + 2788 */
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n > 9970) {
        return (seq_t)n + 2788;
    }
    
    static seq_t *sequence = NULL;
    static idx_t seq_len = 0;
    
    if (sequence == NULL) {
        idx_t limit = 13000;
        bool *can_form = (bool *)malloc((limit + 1) * sizeof(bool));
        memset(can_form, 0, (limit + 1) * sizeof(bool));
        can_form[0] = true;
        
        for (idx_t k = 1; k * k * k <= limit; k++) {
            idx_t cube = k * k * k;
            for (idx_t i = limit; i >= cube; i--) {
                if (can_form[i - cube]) {
                    can_form[i] = true;
                }
            }
        }
        
        seq_t *temp = (seq_t *)malloc(10000 * sizeof(seq_t));
        idx_t count = 0;
        for (idx_t i = 1; i <= limit && count < 10000; i++) {
            if (can_form[i]) {
                temp[count++] = (seq_t)i;
            }
        }
        
        sequence = temp;
        seq_len = count;
        free(can_form);
    }
    
    if (n <= seq_len) {
        return sequence[n - 1];
    }
    
    return (seq_t)n + 2788;
}
