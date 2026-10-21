/*
 * A095100  Integers m of the form 4k+3 for which all sums Sum_{i=1..u} J(i/m)
 *          (with u ranging from 1 to (m-1)) are nonnegative, where J(i/m) is
 *          Jacobi symbol of i and m.
 * https://oeis.org/A095100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A095274(n) + 3
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        (i.e., the n‑th odd integer 4k+3 for which all partial sums of
 *         the Jacobi symbols J(i/m) are non‑negative) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- local Jacobi symbol routine ----- */
static int jacobi_symbol(uint64_t a, uint64_t n)
{
    if (n <= 1) return 0;
    a %= n;
    int s = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            /* Jacobi rule for powers of 2: flip if n ≡ 3 or 5 (mod 8) */
            if ((n & 7) == 3 || (n & 7) == 5) s = -s;
        }
        /* swap a and n */
        uint64_t t = a; a = n; n = t;
        /* quadratic reciprocity: flip sign if both odd and ≡ 3 (mod 4) */
        if ((a & 3) == 3 && (n & 3) == 3) s = -s;
        a %= n;
    }
    return (n == 1) ? s : 0;
}

/* ----- validity test for a candidate m = 4k+3 ----- */
static bool is_valid(uint64_t m)
{
    int64_t sum = 0;
    for (idx_t i = 1; i < m; ++i) {
        int j = jacobi_symbol((uint64_t)i, m);   /* ∈ {-1,0,1} */
        sum += (int64_t)j;
        if (sum < 0) return false;               /* fail early */
    }
    return true;                                 /* all prefixes ≥0 */
}

/* ----- dynamic cache of found terms ----- */
static seq_t *valid_terms = NULL;   /* array of a(1), a(2), ...             */
static size_t valid_len   = 0;      /* number of stored terms               */
static size_t valid_cap   = 0;      /* current allocation capacity          */
static idx_t  next_candidate = 3;   /* next m ≡ 3 (mod 4) to examine        */

/* Append a new valid term, enlarging the array if needed. */
static void append_valid(seq_t val)
{
    if (valid_len == valid_cap) {
        size_t new_cap = valid_cap ? valid_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(valid_terms, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: abort the program (non‑recoverable). */
            abort();
        }
        valid_terms = tmp;
        valid_cap   = new_cap;
    }
    valid_terms[valid_len++] = val;
}

/* ----- Public sequence accessor ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined below offset – harmless */

    /* Ensure the cache contains at least n terms. */
    while (valid_len < (size_t)n) {
        if (is_valid(next_candidate)) {
            append_valid((seq_t)next_candidate);
        }
        next_candidate += 4;   /* next integer of the form 4k+3 */
    }
    return valid_terms[n - 1];
}
