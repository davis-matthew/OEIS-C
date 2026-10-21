/*
 * A080430  a(n) = least odd number such that all pairwise sums a(i) + a(j), i
 *          < j <= n, are distinct.
 * https://oeis.org/A080430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 82, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A080430: a(n) is the least odd number such that all pairwise sums
   of the first n terms are distinct (odd Sidon set). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; 0 is unused */

    /* static state */
    static idx_t cur_n = 0;               /* number of terms already stored */
    static seq_t *a_arr = NULL;           /* array of stored terms */
    static size_t a_cap = 0;              /* capacity of a_arr */

    static uint64_t *sum_bits = NULL;     /* bitset of used sums (32 bits -> 64 bits per word) */
    static size_t sum_cap_words = 0;      /* capacity in words of sum_bits */

    /* Ensure storage for terms up to n */
    if (n > cur_n) {
        size_t new_cap = a_cap ? a_cap : 1;
        while (new_cap < n) new_cap <<= 1;

        if (new_cap > a_cap) {
            seq_t *tmp = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);                     /* allocation failure */
            a_arr = tmp;
            a_cap = new_cap;
        }
    }

    /* Construct terms until reaching n */
    while (cur_n < n) {
        if (cur_n == 0) {                     /* first term */
            a_arr[0] = (seq_t)1;
            cur_n = 1;
            continue;
        }

        seq_t candidate = a_arr[cur_n - 1] + (seq_t)2;   /* next odd candidate */

        /* find the smallest candidate that keeps all pairwise sums distinct */
        for (;;) {
            /* Compute the largest sum that will be inserted for this candidate
               (with the largest existing term). */
            seq_t max_sum = candidate + a_arr[cur_n - 1];
            size_t required_words = ((size_t)max_sum >> 6) + 1;

            /* Grow the bitset if necessary */
            if (required_words > sum_cap_words) {
                size_t new_cap = sum_cap_words ? sum_cap_words : 1;
                while (required_words > new_cap) new_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(sum_bits, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                /* Zero any newly allocated words */
                for (size_t j = sum_cap_words; j < new_cap; ++j) tmp[j] = 0ull;
                sum_bits = tmp;
                sum_cap_words = new_cap;
            }

            /* Test for collisions with existing sums */
            bool ok = true;
            for (idx_t i = 0; i < cur_n; ++i) {
                seq_t s = candidate + a_arr[i];
                size_t idx = ((size_t)s >> 6);
                uint64_t mask = 1ull << ((size_t)s & 63ull);
                if (sum_bits[idx] & mask) { ok = false; break; }
            }
            if (ok) break;                /* candidate accepted */
            candidate += (seq_t)2;        /* try next odd number */
        }

        /* Record the new sums in the bitset */
        for (idx_t i = 0; i < cur_n; ++i) {
            seq_t s = candidate + a_arr[i];
            size_t idx = ((size_t)s >> 6);
            uint64_t mask = 1ull << ((size_t)s & 63ull);
            sum_bits[idx] |= mask;
        }

        a_arr[cur_n] = candidate;
        ++cur_n;
    }

    return a_arr[n - 1];
}
