/*
 * A053523  a(n)=[A*a(n-1)+B*a(n-2)+C]/p^r, where p^r is the highest power of
 *          p dividing [A*a(n-1)+B*a(n-2)+C], A=1.0001, B=1.0001, C=1.9, p=2.
 * https://oeis.org/A053523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1; for n≥3, a(n) = odd part of
 *            ⌊(10001·(a(n-1)+a(n-2)) + 19000) / 10000⌋ where the odd part
 *            removes all factors of 2
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t capacity = 0;

    if (n == 0) return 0;  /* undefined for n<1, but safe return */
    
    /* Initialise first two terms when needed */
    if (size < 2) {
        capacity = (capacity < 2) ? 2 : capacity;
        if (seq == NULL) seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        seq[0] = 1;  /* a(1) */
        seq[1] = 1;  /* a(2) */
        size = 2;
    }

    /* Grow sequence to requested n */
    if ((idx_t)n > size) {
        idx_t new_cap = capacity;
        while ((idx_t)n > new_cap) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        capacity = new_cap;

        for (idx_t i = size; i < (idx_t)n; ++i) {
            seq_t sum = seq[i-1] + seq[i-2];

            /* Compute T = 10001 * sum + 19000 with 128‑bit safety */
            unsigned __int128 tmp = (unsigned __int128)10001 * (unsigned __int128)sum + 19000;
            seq_t val = (seq_t)(tmp / 10000);       /* floor */

            /* Remove all factors of 2: odd part */
            while ((val & 1) == 0 && val != 0) val >>= 1;

            seq[i] = val;
        }
        size = (idx_t)n;
    }

    return seq[n-1];
}
