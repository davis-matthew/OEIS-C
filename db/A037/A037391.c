/*
 * A037391  Numbers k such that every base-4 digit of k is a base-9 digit of
 *          k.
 * https://oeis.org/A037391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2685, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th integer k such that every base‑4 digit of k
   is also a base‑9 digit of k. The function is implemented by
   brute‑force generation of the required numbers. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *cache = NULL;       /* array of values */
    static size_t cache_len = 0;      /* number of values stored */
    static size_t cache_cap = 0;      /* allocated capacity of array */
    static idx_t last_k = 0;          /* largest integer examined so far */

    /* trivial check – the sequence is defined for n>=1 */
    if (n == 0)
        return (seq_t)0;

    /* return cached value if already present */
    if (n <= cache_len)
        return cache[n - 1];

    /* generate candidates until we have at least n terms */
    for (idx_t k = last_k + 1; ; ++k)
    {
        seq_t val = (seq_t)k;

        /* compute digit masks for base‑4 and base‑9 */
        uint32_t mask4 = 0, mask9 = 0;
        seq_t t = val;

        /* digits in base 4 */
        do
        {
            uint32_t d = (uint32_t)(t % (seq_t)4);
            mask4 |= 1u << d;
            t /= 4;
        } while (t);

        /* digits in base 9 */
        t = val;
        do
        {
            uint32_t d = (uint32_t)(t % (seq_t)9);
            mask9 |= 1u << d;
            t /= 9;
        } while (t);

        /* check that every 4‑digit occurs in the 9‑digit set */
        if ((mask4 & ~mask9) == 0)
        {
            /* grow the cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
#ifdef __cplusplus
                seq_t *tmp = static_cast<seq_t *>(
                    realloc(cache, new_cap * sizeof(seq_t)));
#else
                seq_t *tmp = (seq_t *)realloc(cache,
                    new_cap * sizeof(seq_t));
#endif
                if (!tmp)
                    exit(EXIT_FAILURE);          /* allocation failure */

                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = val;
            last_k = k;                /* remember last examined integer */

            if (cache_len >= n)
                return cache[n - 1];
        }
    }
}
