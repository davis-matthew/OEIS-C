/*
 * A086650  n-th composite number raised to the n-th prime number.
 * https://oeis.org/A086650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002808(n)^A000040(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* a(1) is the first term, values for n == 0 are unused */
    if (n == 0) return (seq_t)0;

    /* static cache for composites */
    static idx_t *composites = NULL;
    static size_t comp_cap  = 0;
    static idx_t comp_cnt   = 0;
    static idx_t next_cand  = 4;                /* smallest composite */

    /* grow composite cache as needed */
    if (comp_cnt < n) {
        /* ensure capacity for new entries */
        if (comp_cap < comp_cnt + (size_t)(n - comp_cnt) + 10) {
            size_t new_cap = comp_cap ? comp_cap * 2 : 16;
            while (new_cap < comp_cnt + (size_t)(n - comp_cnt) + 10)
                new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(composites, new_cap * sizeof(idx_t));
            if (!tmp) abort();                    /* allocation failure */
            composites = tmp;
            comp_cap = new_cap;
        }

        /* find composites until we have at least n of them */
        while (comp_cnt < n) {
            while (is_prime(next_cand))
                ++next_cand;
            composites[comp_cnt++] = next_cand;
            ++next_cand;
        }
    }

    idx_t base_idx  = composites[n - 1];
    idx_t prime_idx = get_prime(n);          /* nth prime (idx_t) */

    return big_pow((seq_t)base_idx, (seq_t)prime_idx);
}
