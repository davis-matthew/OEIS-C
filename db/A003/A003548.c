/*
 * A003548  Divisors of 2^43 - 1.
 * https://oeis.org/A003548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^43 - 1, listed in increasing order
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n) {
    static seq_t *divs = NULL;
    static size_t ndivs = 0;
    
    if (!divs) {
        seq_t N = ((seq_t)1 << 43) - 1;
        divs = (seq_t *)malloc(64 * sizeof(seq_t));
        
        for (seq_t i = 1; i <= N / i; i++) {
            if (N % i == 0) {
                divs[ndivs++] = i;
                seq_t q = N / i;
                if (q != i) {
                    divs[ndivs++] = q;
                }
            }
        }
        
        qsort(divs, ndivs, sizeof(seq_t), cmp_seq);
    }
    
    if (n >= 1 && (size_t)n <= ndivs) {
        return divs[n - 1];
    }
    return 0;
}
