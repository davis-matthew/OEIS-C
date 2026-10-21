/*
 * A235724  Squares which have one or more occurrences of exactly nine
 *          different digits.
 * https://oeis.org/A235724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 506, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by exhaustive search for integers whose square has exactly nine distinct digits.
   a(n) = (m_n)^2  where m_n is the nth integer satisfying the digit condition. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Count distinct decimal digits of a value. */
static int count_distinct_digits(seq_t x)
{
    unsigned char seen[10] = {0};
    if (x == 0) {
        seen[0] = 1;
        return 1;
    }
    while (x != 0) {
        int d = (int)(x % 10);
        seen[d] = 1;
        x /= 10;
    }
    int cnt = 0;
    for (int i = 0; i < 10; ++i)
        if (seen[i]) ++cnt;
    return cnt;
}

/* Return the nth element of OEIS A235724. */
seq_t a(idx_t n)
{
    static seq_t *values = NULL;      /* stored a(n) values (squares)          */
    static size_t capacity = 0;       /* current capacity of the array         */
    static idx_t current_size = 0;    /* number of values stored so far        */
    static seq_t next_candidate = 1;  /* next integer to test for property     */

    /* Grow the array if necessary (dynamic array with exponentiation). */
    if (capacity < 64) {
        capacity = 64;
        values = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Build the sequence until we have at least n elements. */
    while (current_size < n) {
        seq_t candidate = next_candidate * next_candidate; /* candidate square   */
        if (count_distinct_digits(candidate) == 9) {
            if (current_size >= capacity) {
                capacity *= 2;
                values = (seq_t *)realloc(values, capacity * sizeof(seq_t));
            }
            values[current_size++] = candidate;
        }
        /* Advance to the next integer. */
        ++next_candidate;
    }

    return values[n - 1];
}
