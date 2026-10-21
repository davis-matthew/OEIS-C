/*
 * A210020  In base 9, numbers n which have 9 distinct digits, do not start
 *          with 0, and have property that the product (written in base 9) of
 *          any two adjacent digits is a substring of n.
 * https://oeis.org/A210020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search of all base‑9 permutations with
   distinct digits, a non‑zero first digit, and the property that
   the product of any two adjacent digits, written in base‑9, is a
   substring of the number. */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of all solutions. */
static seq_t *solutions = NULL;
static size_t sol_len = 0, sol_cap = 0;
static int initialized = 0;

/* Check whether a 9‑digit base‑9 number satisfies the substring
   property.  digits[0..8] are the nine digits. */
static bool satisfies(const int digits[9]) {
    for (int i = 0; i < 8; ++i) {
        int prod = digits[i] * digits[i + 1];
        int len, d1, d2;

        if (prod < 9) {
            len = 1;
            d1 = prod;
        } else {
            len = 2;
            d1 = prod / 9;        /* high base‑9 digit */
            d2 = prod % 9;        /* low  base‑9 digit */
        }

        bool found = false;
        for (int j = 0; j <= 9 - len; ++j) {
            if (len == 1) {
                if (digits[j] == d1) { found = true; break; }
            } else {
                if (digits[j] == d1 && digits[j + 1] == d2) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

/* Ensure capacity for at least `needed` elements. */
static void ensure_cap(size_t needed) {
    if (needed <= sol_cap)
        return;
    size_t newcap = sol_cap ? sol_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (solutions) {
        memcpy(newarr, solutions, sol_len * sizeof(seq_t));
        free(solutions);
    }
    solutions = newarr;
    sol_cap   = newcap;
}

/* Add a 9‑digit number to the solutions cache.  The value is the
   decimal number obtained by concatenating the base‑9 digits. */
static void add_solution(const int digits[9]) {
    seq_t val = 0;
    for (int i = 0; i < 9; ++i)
        val = val * 10 + digits[i];   /* concatenate digits in base‑10 */
    ensure_cap(sol_len + 1);
    solutions[sol_len++] = val;
}

/* Depth‑first traversal of all permutations of digits 0..8. */
static void backtrack(int pos, int used, int digits[9]) {
    if (pos == 9) {
        if (satisfies(digits))
            add_solution(digits);
        return;
    }
    if (pos == 0) {
        for (int d = 1; d <= 8; ++d) {
            if (!(used & (1 << d))) {
                digits[pos] = d;
                backtrack(pos + 1, used | (1 << d), digits);
            }
        }
    } else {
        for (int d = 0; d <= 8; ++d) {
            if (!(used & (1 << d))) {
                digits[pos] = d;
                backtrack(pos + 1, used | (1 << d), digits);
            }
        }
    }
}

/* Comparator for sorting seq_t values. */
static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Populate the solutions array lazily. */
static void initialize(void) {
    int digits[9];
    backtrack(0, 0, digits);
    qsort(solutions, sol_len, sizeof(seq_t), cmp_seq);
    initialized = 1;
}

/* Public interface: a(n) returns the nth term (1‑based). */
seq_t a(idx_t n) {
    if (!initialized) initialize();
    if (n == 0 || (idx_t)n > sol_len)
        return 0;
    return solutions[n - 1];
}
