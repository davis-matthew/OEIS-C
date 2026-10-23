/*
 * A137007  Numbers k such that k and k^2 use only the digits 1, 2, 5 and 7.
 * https://oeis.org/A137007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_frequencies, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A137007:  
   k such that k and k² use only the digits 1,2,5,7.
   The term a(n) is found by breadth‑first enumeration of
   numbers with allowed digits and testing the square of each candidate. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"          /* seq_mul(seq_t a, seq_t b) */
#include "../utils/digit_frequencies.h"/* digit_frequencies(seq_t x, unsigned int freq[10]) */

/* Ensure a dynamic array has capacity for at least 'needed' elements.  */
static void _ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Test whether all decimal digits of x belong to {1,2,5,7}.  */
static bool _digits_allowed(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    return freq[0] == 0 && freq[3] == 0 &&
           freq[4] == 0 && freq[6] == 0 &&
           freq[8] == 0 && freq[9] == 0;
}

/* Main function returning the n‑th term (1‑based).  */
seq_t a(idx_t n)
{
    /* Static storage for discovered valid terms and the BFS queue.  */
    static seq_t *valid      = NULL;  /* valid terms */
    static size_t valid_len  = 0;
    static size_t valid_cap  = 0;

    static seq_t *queue      = NULL;  /* BFS queue */
    static size_t queue_len  = 0;
    static size_t queue_cap  = 0;
    static size_t queue_pos  = 0;

    /* One-time initialization.  */
    if (valid == NULL) {
        _ensure_capacity(&valid, &valid_cap, 16);
        _ensure_capacity(&queue, &queue_cap, 16);

        /* Initial 1‑digit numbers that use only the allowed digits.  */
        queue[0] = (seq_t)1;
        queue[1] = (seq_t)2;
        queue[2] = (seq_t)5;
        queue[3] = (seq_t)7;
        queue_len = 4;
    }

    /* Enumerate numbers until we have at least n valid terms.  */
    while (valid_len < n) {
        if (queue_pos >= queue_len) {
            /* The queue should never exhaust before we find n terms,
               but guard against it to avoid a crash.  */
            break;
        }

        seq_t cur = queue[queue_pos++];

        /* Test whether the square of cur has only allowed digits.  */
        seq_t sq = seq_mul(cur, cur);
        if (_digits_allowed(sq)) {
            _ensure_capacity(&valid, &valid_cap, valid_len + 1);
            valid[valid_len++] = cur;
            /* We do not need to test cur itself—the queue only contains
               numbers whose decimal digits are already in the allowed set.  */
        }

        /* Generate the next level of the breadth‑first search.  */
        seq_t base = seq_mul(cur, (seq_t)10);
        const seq_t digits[4] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)7 };
        for (size_t i = 0; i < 4; ++i) {
            seq_t child = base + digits[i];
            _ensure_capacity(&queue, &queue_cap, queue_len + 1);
            queue[queue_len++] = child;
        }
    }

    /* Return the requested term (1‑based indexing).  */
    return valid[n - 1];
}
