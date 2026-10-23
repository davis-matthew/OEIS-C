/*
 * A144833  Lucas numbers with an equal number of odd and even digits.
 * https://oeis.org/A144833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Lucas numbers that have an equal number of odd and even digits.
 *            The function a(n) returns the n-th such Lucas number (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 12, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   digit_counts, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order calls and caching. */
    static bool initialized = false;
    static seq_t L_prev = 2;   /* L0 */
    static seq_t L_curr = 1;   /* L1 */
    static idx_t  current_index = 0;   /* next Lucas index to test */
    static seq_t *valid = NULL;        /* array of found values */
    static size_t stored = 0;          /* number of values stored */
    static size_t capacity = 0;        /* current array capacity */

    if (!initialized)
    {
        initialized = true;
        L_prev = 2;
        L_curr = 1;
        current_index = 0;
        valid = NULL;
        stored = 0;
        capacity = 0;
    }

    /* Generate values until we have at least n terms. */
    while (stored < (size_t)n)
    {
        seq_t cur_value;
        if (current_index < 2)
            cur_value = (current_index == 0) ? L_prev : L_curr;
        else
        {
            seq_t next = L_prev + L_curr;
            L_prev = L_curr;
            L_curr = next;
            cur_value = L_curr;
        }

        /* Count digits. */
        unsigned char cnt[10];
        for (size_t i = 0; i < 10; ++i) cnt[i] = 0;
        digit_counts(cur_value, cnt);

        unsigned int even = cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8];
        unsigned int odd  = cnt[1] + cnt[3] + cnt[5] + cnt[7] + cnt[9];

        if (even == odd)
        {
            if (stored == capacity)
                grow_seq_array(&valid, &capacity, stored + 1);
            valid[stored] = cur_value;
            ++stored;
        }

        ++current_index;
    }

    return valid[n - 1];
}
