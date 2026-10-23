/*
 * A036580  Ternary Thue-Morse sequence: closed under a->abc, b->ac, c->b.
 * https://oeis.org/A036580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Recurrence: start with a (0), then apply the morphism
   a → abc  (0 → 0,1,2)
   b → ac   (1 → 0,2)
   c → b    (2 → 1)
   The sequence gives the 0/1/2 value of the letter at index n. */

seq_t a(idx_t n)
{
    static uint8_t *cache = NULL;   /* cached terms as 0/1/2 values */
    static size_t cache_size = 0;   /* number of cached terms */

    if (cache == NULL) {
        cache = (uint8_t*)malloc(sizeof(uint8_t));
        cache[0] = 0;               /* a(0) = 0 (letter 'a') */
        cache_size = 1;
    }

    /* Expand until we have enough terms for index n. */
    while (cache_size <= (size_t)n) {
        /* Determine the length after one substitution. */
        size_t new_len = 0;
        for (size_t i = 0; i < cache_size; ++i) {
            uint8_t val = cache[i];
            if (val == 0)       new_len += 3;   /* a → abc */
            else if (val == 1)  new_len += 2;   /* b → ac  */
            else               new_len += 1;   /* c → b   */
        }

        uint8_t *new_cache = (uint8_t*)malloc(new_len * sizeof(uint8_t));
        size_t pos = 0;
        for (size_t i = 0; i < cache_size; ++i) {
            uint8_t val = cache[i];
            switch (val) {
                case 0: new_cache[pos++] = 0; new_cache[pos++] = 1; new_cache[pos++] = 2; break;
                case 1: new_cache[pos++] = 0; new_cache[pos++] = 2; break;
                case 2: new_cache[pos++] = 1; break;
                default: break; /* should not happen */
            }
        }

        free(cache);
        cache = new_cache;
        cache_size = new_len;
    }

    return (seq_t)cache[n];
}
