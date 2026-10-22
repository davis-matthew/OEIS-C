/*
 * A004823  Numbers that are the sum of 12 positive 11th powers.
 * https://oeis.org/A004823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   cmp_vals, pow11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_vals.h"
#include "../utils/pow11.h"

/* a(n) = n-th smallest sum of 12 positive 11th powers */

seq_t a(idx_t n) {
    static seq_t *sums = NULL;
    static idx_t count = 0;
    
    if (sums == NULL) {
        idx_t capacity = 2000000;
        sums = (seq_t *)malloc(capacity * sizeof(seq_t));
        idx_t idx = 0;
        
        seq_t MAX_BASE = 12;
        
        for (seq_t b1 = 1; b1 <= MAX_BASE; b1++) {
            seq_t p1 = pow11(b1);
            for (seq_t b2 = b1; b2 <= MAX_BASE; b2++) {
                seq_t p2 = p1 + pow11(b2);
                for (seq_t b3 = b2; b3 <= MAX_BASE; b3++) {
                    seq_t p3 = p2 + pow11(b3);
                    for (seq_t b4 = b3; b4 <= MAX_BASE; b4++) {
                        seq_t p4 = p3 + pow11(b4);
                        for (seq_t b5 = b4; b5 <= MAX_BASE; b5++) {
                            seq_t p5 = p4 + pow11(b5);
                            for (seq_t b6 = b5; b6 <= MAX_BASE; b6++) {
                                seq_t p6 = p5 + pow11(b6);
                                for (seq_t b7 = b6; b7 <= MAX_BASE; b7++) {
                                    seq_t p7 = p6 + pow11(b7);
                                    for (seq_t b8 = b7; b8 <= MAX_BASE; b8++) {
                                        seq_t p8 = p7 + pow11(b8);
                                        for (seq_t b9 = b8; b9 <= MAX_BASE; b9++) {
                                            seq_t p9 = p8 + pow11(b9);
                                            for (seq_t b10 = b9; b10 <= MAX_BASE; b10++) {
                                                seq_t p10 = p9 + pow11(b10);
                                                for (seq_t b11 = b10; b11 <= MAX_BASE; b11++) {
                                                    seq_t p11 = p10 + pow11(b11);
                                                    for (seq_t b12 = b11; b12 <= MAX_BASE; b12++) {
                                                        if (idx < capacity) {
                                                            sums[idx++] = p11 + pow11(b12);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        qsort(sums, idx, sizeof(seq_t), cmp_vals);
        
        idx_t write = 0;
        for (idx_t i = 0; i < idx; i++) {
            if (i == 0 || sums[i] != sums[i-1]) {
                sums[write++] = sums[i];
            }
        }
        count = write;
    }
    
    return (n <= count) ? sums[n - 1] : 0;
}
