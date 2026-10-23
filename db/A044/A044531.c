/*
 * A044531  Numbers n such that string 1,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑7 representation of
 *            k contains the substring "15" but the base‑7 representation of
 *            k+1 does not contain that substring
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: test if a number contains the substring 1,5 in base‑7. */
static bool has15(seq_t x)
{
    if (x == (seq_t)0) return false;

    int prev = -1;
    /* Scan digits from least‑significant to most‑significant.  A
       substring "15" in normal order appears as the pair 5,1 in
       reversed digit order. */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)7);
        if (prev == 5 && d == 1) return true;
        prev = d;
        x /= (seq_t)7;
    }
    return false;
}

/* Dynamic array of computed terms, with simple memoisation. */
static seq_t *sequence = NULL;      /* array of stored terms */
static idx_t  seq_count  = 0;       /* number of terms stored */
static size_t seq_cap    = 0;       /* allocated capacity */
static seq_t  test_num   = 0;       /* next integer to test */

/* Ensure the array has at least ``need`` slots. */
static void ensure_capacity(size_t need)
{
    if (need > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *new_arr = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        sequence = new_arr;
        seq_cap = new_cap;
    }
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Lazily generate terms up to the requested index. */
    while ((idx_t)seq_count < n) {
        ++test_num;                /* next candidate integer */
        if (has15(test_num) && !has15(test_num + (seq_t)1)) {
            ensure_capacity(seq_count + 1);
            sequence[seq_count++] = test_num;
        }
    }
    /* seq_count >= n now; return 0‑based index. */
    return sequence[n - 1];
}
