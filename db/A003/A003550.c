/*
 * A003550  Divisors of 2^45 - 1.
 * https://oeis.org/A003550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 2^45 - 1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 42, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: full, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int cmp_div(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t *divs = NULL;
    static idx_t cnt = 0;
    static int init = 0;
    
    if (!init) {
        init = 1;
        uint64_t N = ((uint64_t)1 << 45) - 1;
        
        idx_t cap = 100;
        divs = mem_new(seq_t, cap);
        
        for (uint64_t i = 1; i * i <= N; i++) {
            if (N % i == 0) {
                if (cnt >= cap) {
                    cap *= 2;
                    divs = mem_resize(seq_t, divs, cap);
                }
                divs[cnt++] = (seq_t)i;
                
                uint64_t j = N / i;
                if (i != j) {
                    if (cnt >= cap) {
                        cap *= 2;
                        divs = mem_resize(seq_t, divs, cap);
                    }
                    divs[cnt++] = (seq_t)j;
                }
            }
        }
        
        mem_qsort(divs, cnt, sizeof(seq_t), cmp_div);
    }
    
    return divs[n - 1];
}
