/*
 * A000797  Numbers that are not the sum of 4 tetrahedral numbers.
 * https://oeis.org/A000797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   Numbers that cannot be expressed as the sum of 4 tetrahedral
 *            numbers. Tetrahedral number T(k) = k*(k+1)*(k+2)/6. Exactly 241
 *            such numbers exist, largest is 343867
 * Range:     u8 none, u16 none, u32 n <= 241, u64 n <= 241, u128 n <= 241,
 *            big any n
 * Helpers:   tetrahedral
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/tetrahedral.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (cache == NULL) {
        const idx_t MAX_VAL = 350000;
        
        bool *can_express = (bool *)calloc(MAX_VAL + 1, sizeof(bool));
        
        seq_t *tet = (seq_t *)malloc(150 * sizeof(seq_t));
        idx_t tet_count = 0;
        for (idx_t k = 0; k < 150; k++) {
            seq_t t = tetrahedral(k);
            if ((idx_t)t > MAX_VAL) break;
            tet[tet_count++] = t;
        }
        
        for (idx_t i1 = 0; i1 < tet_count; i1++) {
            seq_t s1 = tet[i1];
            for (idx_t i2 = i1; i2 < tet_count; i2++) {
                seq_t s2 = s1 + tet[i2];
                if ((idx_t)s2 > MAX_VAL) break;
                for (idx_t i3 = i2; i3 < tet_count; i3++) {
                    seq_t s3 = s2 + tet[i3];
                    if ((idx_t)s3 > MAX_VAL) break;
                    for (idx_t i4 = i3; i4 < tet_count; i4++) {
                        seq_t s4 = s3 + tet[i4];
                        if ((idx_t)s4 > MAX_VAL) break;
                        can_express[(idx_t)s4] = true;
                    }
                }
            }
        }
        
        cache = (seq_t *)malloc(242 * sizeof(seq_t));
        idx_t idx = 0;
        for (idx_t cand = 1; cand <= MAX_VAL && idx < 241; cand++) {
            if (!can_express[cand]) {
                cache[idx++] = (seq_t)cand;
            }
        }
        cache_size = idx;
        
        free(can_express);
        free(tet);
    }
    
    if (n >= 1 && n <= cache_size) {
        return cache[n - 1];
    }
    return 0;
}
