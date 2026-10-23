/*
 * A106250  Expansion of (1-x+x^2+x^3)/(1-x-x^5+x^6).
 * https://oeis.org/A106250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=1, a(3)=2;
   for n>=4, a(n)=a(n-1)+a(n-5)-a(n-6) */
#include <stdlib.h>

static seq_t *a_arr = NULL;
static idx_t a_size = 0;

seq_t a(idx_t n) {
    /* initialize the table if empty */
    if (a_size == 0) {
        a_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!a_arr) exit(1);            /* allocation failed */
        a_arr[0] = (seq_t)1;            /* a(0) */
        a_size = 1;
    }
    /* return cached value if already computed */
    if (n < a_size)
        return a_arr[n];

    /* grow the table up to n */
    idx_t new_size = n + 1;
    a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
    if (!a_arr) exit(1);                /* allocation failed */

    /* compute new entries */
    for (idx_t i = a_size; i <= n; ++i) {
        seq_t val = 0;
        switch (i) {
            case 0: val = (seq_t)1; break;
            case 1: val = (seq_t)0; break;
            case 2: val = (seq_t)1; break;
            case 3: val = (seq_t)2; break;
            default:
                val = a_arr[i - 1];
                if (i >= 5) val += a_arr[i - 5];
                if (i >= 6) val -= a_arr[i - 6];
        }
        a_arr[i] = val;
    }
    a_size = new_size;
    return a_arr[n];
}
