/*
 * A136785  Number of multiplex juggling sequences of length n, base state
 *          <2,1> and hand capacity 3.
 * https://oeis.org/A136785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=5, a(3)=30, a(4)=182;
 *            a(n)=10*a(n-1)-27*a(n-2)+20*a(n-3) for n>4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *data = NULL;
static size_t data_cap = 0;
static idx_t stored = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    /* Ensure the array can hold up to n elements */
    if (data_cap < (size_t)n) {
        size_t new_cap = data_cap ? data_cap * 2 : 4;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = seq_alloc_array(new_cap);
        if (data != NULL) {
            for (idx_t i = 0; i < stored; ++i) tmp[i] = data[i];
            seq_free_array(data);
        }
        data = tmp;
        data_cap = new_cap;
    }

    /* Compute terms that are not yet stored */
    for (idx_t i = stored; i < n; ++i) {
        if (i == 0) {
            data[0] = (seq_t)1;
        } else if (i == 1) {
            data[1] = (seq_t)5;
        } else if (i == 2) {
            data[2] = (seq_t)30;
        } else if (i == 3) {
            data[3] = (seq_t)182;
        } else {
            seq_t t1 = data[i - 1];
            seq_t t2 = data[i - 2];
            seq_t t3 = data[i - 3];
            seq_t term1 = (seq_t)10 * t1;
            seq_t term2 = (seq_t)27 * t2;
            seq_t term3 = (seq_t)20 * t3;
            data[i] = term1 - term2 + term3;
        }
    }

    stored = n;
    return data[n - 1];
}
