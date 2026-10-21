/*
 * A061384  Numbers n such that sum of digits = number of digits.
 * https://oeis.org/A061384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number n whose sum of digits equals the number of digits
 * Range:     u8 n <= 8, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Internal storage for the sequence, growing by digit length. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TERMS 50000   /* enough for n up to at least 10000 */

static seq_t   terms[MAX_TERMS];
static size_t  terms_len  = 0;   /* number of terms stored */
static int     generated_d = 0;  /* last digit length generated */

/* Append a term to the table (safely ignore overflow). */
static inline void append_term(seq_t v)
{
    if (terms_len < MAX_TERMS)
        terms[terms_len++] = v;
}

/* Recursive generation of all d‑digit numbers with digit sum = d.
   The digits are constructed from most significant to least, which
   guarantees numeric order for a fixed d. */
static void rec_generate(int pos, int sum_left, int d, seq_t cur)
{
    if (pos == d) {
        if (sum_left == 0)
            append_term(cur);
        return;
    }

    int min_digit = (pos == 0) ? 1 : 0;
    int max_digit = sum_left < 9 ? sum_left : 9;
    int remaining_positions = d - pos - 1;

    for (int digit = min_digit; digit <= max_digit; ++digit) {
        int new_sum = sum_left - digit;
        if (new_sum > 9 * remaining_positions)
            continue; /* cannot be satisfied by remaining positions */
        rec_generate(pos + 1, new_sum, d, cur * (seq_t)10 + (seq_t)digit);
    }
}

static void generate_digit_length(int d)
{
    rec_generate(0, d, d, (seq_t)0);
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* offset is 1, behaviour for n == 0 is arbitrary */

    while (terms_len < (size_t)n) {
        generated_d++;                /* next digit length to generate */
        generate_digit_length(generated_d);
    }

    return terms[n - 1];
}
