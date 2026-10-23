/*
 * A195666  Numbers whose sum of prime factors is 17.
 * https://oeis.org/A195666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose sum of prime factors (with multiplicity) equals 17.
   The function generates all products of primes {2,3,5,7,11,13,17}
   whose exponents satisfy 2·e2 + 3·e3 + 5·e5 + 7·e7 + 11·e11 + 13·e13 + 17·e17 = 17.
   The generated values are sorted and cached; a(idx_t n) returns the n‑th
   value of this sequence for n≥1.  The same code works for both
   fixed‑size integer and arbitrary‑precision builds. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static const uint64_t prime_list[7] = {2, 3, 5, 7, 11, 13, 17};

/* A static buffer that is large enough for all numbers that satisfy
   the condition.  The sequence is tiny (≤ 30 elements), so a modest
   static size is safe. */
#define MAX_SIZE 128
static seq_t seq[MAX_SIZE];
static size_t seq_len = 0;

/* Append a value to the static array (no reallocation needed). */
static void append_seq(seq_t val) {
    if (seq_len < MAX_SIZE) {
        seq[seq_len++] = val;
    }
}

/* Recursive generation of all multisets of primes with sum 17.
   idx  : index in prime_list (ascending order)
   rem  : remaining sum to be achieved
   prod : current product of chosen primes */
static void generate_rec(size_t idx, uint64_t rem, seq_t prod) {
    if (idx == 7) {                     /* all primes considered */
        if (rem == 0 && prod != (seq_t)0)     /* all sum used, product non‑zero */
            append_seq(prod);
        return;
    }
    uint64_t p = prime_list[idx];
    seq_t prod_iter = prod;
    for (uint64_t cnt = 0; cnt * p <= rem; ++cnt) {
        generate_rec(idx + 1, rem - cnt * p, prod_iter);
        prod_iter *= (seq_t)p;          /* multiply by p for next cnt */
    }
}

/* Comparator for qsort – returns negative, zero or positive */
static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Ensure the sequence is generated and sorted; idempotent. */
static void ensure_sequence(void) {
    if (seq_len == 0) {
        generate_rec(0, 17, (seq_t)1);
        qsort(seq, seq_len, sizeof(seq_t), cmp_seq);
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* undefined for n == 0, must not happen */
    ensure_sequence();
    if ((idx_t)n > (idx_t)seq_len) return (seq_t)0; /* out of range – should not be requested */
    return seq[(size_t)(n - 1)];
}
