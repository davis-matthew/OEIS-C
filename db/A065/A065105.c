/*
 * A065105  Numbers not expressible as a product of Fibonacci numbers.
 * https://oeis.org/A065105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A065105: numbers not expressible as a product of
   Fibonacci numbers. The function builds the set of all products of
   Fibonacci numbers up to a computed bound and scans for the nth
   non‑representable integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0; offset is 1 */

    /* Target count */
    size_t target = (size_t)n;

    /* Initial upper bound guess; scaled to ensure enough terms. */
    size_t B = target * 6 + 1000;
    if (B < 300) B = 300;          /* small safety margin for tiny n */

    while (1) {
        /* Mark which numbers up to B are representable as product of Fibonacci numbers. */
        uint8_t *rep = (uint8_t *)calloc(B + 1, 1);          /* false/0 init */
        if (!rep) abort();

        /* Generate Fibonacci numbers >= 2 up to B. */
        uint64_t *fibs = NULL;
        size_t fibs_cnt = 0, fibs_cap = 0;
        uint64_t a0 = 1, a1 = 1;
        while (1) {
            uint64_t a2 = a0 + a1;
            if (a2 > B) break;
            if (fibs_cnt == fibs_cap) {
                fibs_cap = fibs_cap ? fibs_cap * 2 : 8;
                fibs = (uint64_t *)realloc(fibs, fibs_cap * sizeof(uint64_t));
                if (!fibs) abort();
            }
            fibs[fibs_cnt++] = a2;  /* store Fibonacci number >= 2 */
            a0 = a1;
            a1 = a2;
        }

        /* Breadth‑first generation of representable numbers. */
        uint64_t *queue = (uint64_t *)malloc((B + 1) * sizeof(uint64_t));
        if (!queue) abort();
        size_t head = 0, tail = 0;

        rep[1] = 1;                    /* 1 is trivially representable */
        queue[tail++] = 1;

        while (head < tail) {
            uint64_t cur = queue[head++];
            for (size_t i = 0; i < fibs_cnt; ++i) {
                uint64_t f = fibs[i];
                unsigned __int128 prod = (unsigned __int128)cur * f;
                if (prod > B) continue;
                uint64_t y = (uint64_t)prod;
                if (!rep[y]) {
                    rep[y] = 1;
                    queue[tail++] = y;
                }
            }
        }

        /* Scan numbers to collect non‑representable ones. */
        size_t found = 0;
        for (size_t x = 1; x <= B; ++x) {
            if (!rep[x]) {
                ++found;
                if (found == target) {
                    seq_t result = (seq_t)x;

                    /* Clean up */
                    free(rep);
                    free(queue);
                    free(fibs);
                    return result;
                }
            }
        }

        /* Not enough terms found; increase bound and retry. */
        free(rep);
        free(queue);
        free(fibs);
        B *= 2;                      /* double the bound and try again */
    }
}
