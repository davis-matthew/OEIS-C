/*
 * A053056  Fibonacci numbers whose digit sum is also a Fibonacci number.
 * https://oeis.org/A053056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   grow_bool_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth Fibonacci number whose digit sum is also a Fibonacci number */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/grow_bool_array.h"
#include "../utils/grow_seq_array.h"

/* Main function to return the nth term (1‑based). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    /* Storage for the generated sequence */
    static seq_t *seq = NULL;          /* array of sequence values */
    static size_t seq_len = 0, seq_cap = 0;
    /* To avoid storing the duplicate 1 */
    static seq_t last_added = (seq_t)-1;
    /* Small Fibonacci numbers used to check digit sums */
    static uint64_t *small_fibs = NULL;
    static size_t small_fib_len = 0, small_fib_cap = 0;
    /* Boolean lookup for small Fibonacci numbers */
    static bool *is_fib = NULL;
    static size_t is_fib_cap = 0;
    /* Current Fibonacci numbers */
    static seq_t fib_prev = 0, fib_curr = 1;

    /* One‑time initialization */
    if (!initialized) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = 0;
        seq_len = 1;
        last_added = 0;

        small_fib_cap = 4;
        small_fibs = (uint64_t *)malloc(small_fib_cap * sizeof(uint64_t));
        if (!small_fibs) abort();
        small_fibs[0] = 0;
        small_fibs[1] = 1;
        small_fib_len = 2;

        is_fib_cap = 16;
        is_fib = (bool *)malloc(is_fib_cap * sizeof(bool));
        if (!is_fib) abort();
        for (size_t i = 0; i < is_fib_cap; ++i) is_fib[i] = false;
        is_fib[0] = true;
        is_fib[1] = true;

        initialized = true;
    }

    /* Generate sequence until the nth term is available */
    while (seq_len < (size_t)n) {
        /* Compute digit sum of fib_curr */
        idx_t digit_sum = 0;
        seq_t temp = fib_curr;
        do {
            digit_sum += (idx_t)(temp % (seq_t)10);
            temp /= (seq_t)10;
        } while (temp != 0);

        /* Extend small_fibs and is_fib to cover digit_sum if necessary */
        while (small_fib_len < 2 || small_fibs[small_fib_len - 1] < (uint64_t)digit_sum) {
            uint64_t next = small_fibs[small_fib_len - 1] + small_fibs[small_fib_len - 2];

            if (small_fib_len == small_fib_cap) {
                small_fib_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(small_fibs, small_fib_cap * sizeof(uint64_t));
                if (!tmp) abort();
                small_fibs = tmp;
            }
            small_fibs[small_fib_len] = next;

            if (next >= is_fib_cap)
                grow_bool_array(&is_fib, &is_fib_cap, (size_t)next);

            is_fib[(size_t)next] = true;
            ++small_fib_len;
        }

        /* Check if digit_sum is Fibonacci */
        if (digit_sum < is_fib_cap && is_fib[(size_t)digit_sum]) {
            if (fib_curr != last_added) {
                grow_seq_array(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = fib_curr;
                last_added = fib_curr;
            }
        }

        /* Advance Fibonacci sequence */
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }

    return seq[n - 1];
}
