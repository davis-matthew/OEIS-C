/*
 * A169294  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=41, a(i)=40 · a(i‑1) for 2≤i≤28 Recurrence for
 *            n≥29: a(n) = 39 · a(n‑1)+…+39 · a(n‑28) – 780 · a(n‑29)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache    = NULL;
    static idx_t size      = 0;       /* first invalid index after stored terms */
    static idx_t capacity  = 0;       /* number of allocated slots */

    /* --- initialisation ---------------------------------------------------- */
    if (capacity == 0) {
        capacity = 30;                       /* at least 29 slots (0…28) */
        cache = alloc_seq_array(capacity);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)41;
        for (idx_t i = 2; i <= 28; ++i)
            cache[i] = cache[i-1] * (seq_t)40;   /* a(i) = 40·a(i‑1)  */

        size = 29;                              /* indices 0…28 are valid */
    }

    /* --- grow cache if necessary ------------------------------------------ */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;     /* double until big enough */

        seq_t *tmp = alloc_seq_array(new_cap);
        for (idx_t i = 0; i < size; ++i)
            tmp[i] = cache[i];

        free_seq_array(cache);
        cache    = tmp;
        capacity = new_cap;
    }

    /* --- compute missing terms using the recurrence ---------------------- */
    if (n >= size) {
        const seq_t C39  = (seq_t)39;
        const seq_t C780 = (seq_t)780;

        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = 0;
            /* 39·a(i‑k) for k=1…28 */
            for (idx_t k = 1; k <= 28; ++k)
                sum += cache[i-k] * C39;

            /* –780·a(i‑29) */
            sum -= cache[i-29] * C780;

            cache[i] = sum;
        }

        size = n + 1;   /* now cache[0…n] are valid */
    }

    return cache[n];
}
