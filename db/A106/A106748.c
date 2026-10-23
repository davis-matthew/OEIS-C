/*
 * A106748  Define the morphism f: 1->113, 2->13, 3->2 and let a(0) = 1; then
 *          a(n+1) = f(a(n)).
 * https://oeis.org/A106748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1; a(n+1)=f(a(n)) where f(1)=113, f(2)=13, f(3)=2 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;        /* base case */

    /* current sequence as a string of digits */
    idx_t cur_len = 1;
    char *cur = (char*)malloc(cur_len);
    cur[0] = '1';

    /* iterate n times */
    for (idx_t step = 0; step < n; ++step) {
        /* compute length of next string */
        idx_t next_len = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            switch (cur[i]) {
            case '1': next_len += 3; break; /* 113 */
            case '2': next_len += 2; break; /* 13  */
            case '3': next_len += 1; break; /* 2   */
            }
        }

        /* build next string */
        char *next = (char*)malloc(next_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            switch (cur[i]) {
            case '1':
                next[pos++] = '1';
                next[pos++] = '1';
                next[pos++] = '3';
                break;
            case '2':
                next[pos++] = '1';
                next[pos++] = '3';
                break;
            case '3':
                next[pos++] = '2';
                break;
            }
        }

        free(cur);
        cur = next;
        cur_len = next_len;
    }

    /* convert final string to seq_t value */
    seq_t val = 0;
    for (idx_t i = 0; i < cur_len; ++i) {
        val = val * 10 + (seq_t)(cur[i] - '0');
    }

    free(cur);
    return val;
}
