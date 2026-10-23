/*
 * A014887  n is equal to the number of 1's in all numbers <= n written in
 *          base 7.
 * https://oeis.org/A014887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n-th integer such that the number of digit 1's in
 *            the base‑7 representations of all numbers ≤ a(n) equals a(n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *solutions = NULL;
static size_t sol_len = 0, sol_cap = 0;
static idx_t cur_n = 0;         /* last number processed */
static seq_t cur_ones = 0;      /* total ones in all numbers <= cur_n */
static seq_t ones_in_num = 0;   /* ones in current number cur_n */
static uint8_t digits[100];     /* base‑7 digits of cur_n (least significant first) */
static int dig_len = 1;         /* number of populated digits          */
static bool initialized = false;

/* Grow the solutions array to at least new_cap elements. */
static void grow_solutions(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* allocation failure */
    solutions = tmp;
    sol_cap = new_cap;
}

/* One‑time initialization of static data. */
static void init_once(void)
{
    dig_len = 1;
    digits[0] = 0;
    cur_n = 0;
    cur_ones = 0;
    ones_in_num = 0;

    if (sol_cap == 0) {
        sol_cap = 64;
        solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));
        if (!solutions) exit(1);
    }
    sol_len = 0;
    initialized = true;
}

/* Return the n-th term of A014887 (1‑based). */
seq_t a(idx_t n)
{
    if (!initialized) init_once();

    if (n <= sol_len) return solutions[n - 1];

    /* Continue generating numbers until we have at least n solutions. */
    while (sol_len < n) {
        /* Increment cur_n by one in base 7, updating digits and ones count. */
        int i = 0;
        while (1) {
            uint8_t old = digits[i];
            if (old < 6) {
                uint8_t newd = old + 1;
                digits[i] = newd;
                if (old == 1) ones_in_num--;      /* removed a 1 */
                if (newd == 1) ones_in_num++;     /* added a 1 */
                break;
            } else { /* old == 6, carry to next digit */
                digits[i] = 0;
                /* old != 1, so ones_in_num unchanged */
                i++;
                if (i == dig_len) {
                    digits[i] = 0;
                    dig_len++;
                }
            }
        }

        cur_n++;                      /* new number */
        cur_ones += ones_in_num;      /* total ones up to cur_n */

        if (cur_ones == (seq_t)cur_n) {
            if (sol_len == sol_cap) {
                size_t new_cap = sol_cap * 2;
                if (new_cap == 0) new_cap = 64;
                grow_solutions(new_cap);
            }
            solutions[sol_len++] = (seq_t)cur_n;
        }
    }

    return solutions[n - 1];
}
