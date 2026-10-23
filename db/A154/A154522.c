/*
 * A154522  Numbers n such that number of occurrences of the smallest digit in
 *          prime(n) is 2.
 * https://oeis.org/A154522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th index k such that the smallest digit in prime(k)
 *            appears exactly twice. The function keeps a memoised table of
 *            all indices found so far
 * Range:     u8 n <= 11, u16 n <= 1735, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k); */
#include "../utils/digit_frequencies.h" /* void digit_frequencies(seq_t x, unsigned int freq[10]); */

static idx_t *qual = NULL;   /* array of qualifying indices (prime indices) */
static size_t qual_len = 0;  /* number of stored qualifying indices      */
static size_t qual_cap = 0;  /* allocated capacity of qual array         */
static idx_t last_checked = 0; /* last prime index that has been examined */

static void ensure_capacity(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();
        *ptr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, safe to return 0 */

    /* generate qualifying indices until we have at least n of them */
    while (qual_len < n) {
        idx_t next_idx = last_checked + 1;
        uint32_t prime_val = get_prime(next_idx);

        unsigned int freq[10] = {0};
        digit_frequencies((seq_t)prime_val, freq);

        int smallest = -1;
        for (int d = 0; d <= 9; ++d) {
            if (freq[d] > 0) { smallest = d; break; }
        }

        if (smallest >= 0 && freq[smallest] == 2) {
            ensure_capacity((void**)&qual, &qual_cap, qual_len + 1, sizeof(idx_t));
            qual[qual_len++] = next_idx;
        }

        last_checked = next_idx;
    }

    return (seq_t)qual[n - 1];
}
