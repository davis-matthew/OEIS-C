/*
 * A043923  Numbers k such that 2 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the base‑7
 *            representation of k contains the adjacent digits 2 and 4 in
 *            either order, and the base‑7 representation of k+1 does not
 *            contain such an adjacent pair. The function searches numbers
 *            incrementally, caching results for repeated calls
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached sequence terms */
    static size_t cache_cap = 0;         /* current capacity of cache */
    static size_t cache_size = 0;        /* number of terms stored */
    static seq_t next_candidate = 1;     /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        seq_t k = next_candidate++;          /* candidate integer */

        /* check if k contains an adjacent pair 2‑4 or 4‑2 */
        bool has_adj_k = false;
        {
            seq_t x = k;
            int prev = -1;
            while (x > 0) {
                int digit = (int)(x % 7);
                if ((prev == 2 && digit == 4) || (prev == 4 && digit == 2)) {
                    has_adj_k = true;
                    break;
                }
                prev = digit;
                x = x / 7;
            }
        }
        if (!has_adj_k) continue;  /* k doesn't satisfy the condition */

        /* check if k+1 contains such an adjacent pair (it must not) */
        bool has_adj_k1 = false;
        {
            seq_t x = k + (seq_t)1;
            int prev = -1;
            while (x > 0) {
                int digit = (int)(x % 7);
                if ((prev == 2 && digit == 4) || (prev == 4 && digit == 2)) {
                    has_adj_k1 = true;
                    break;
                }
                prev = digit;
                x = x / 7;
            }
        }
        if (has_adj_k1) continue;  /* k+1 violates the condition */

        /* store k in the cache, enlarging it if needed */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = k;
    }

    return cache[n - 1];
}
