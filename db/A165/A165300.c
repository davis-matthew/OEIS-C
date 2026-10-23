/*
 * A165300  a(n) is the smallest number not already present that permits the
 *          cyclic repetition of the path 1,2 of the digits in the sequence.
 * https://oeis.org/A165300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A165300
 *  Computed by iteratively appending the smallest unused prefix of
 *  the infinite alternating word 1,2,1,2,… .
 *  a(1) = 1, a(2) = 2, a(3) = 12, a(4) = 121, a(5) = 21, …
 *
 *  The algorithm stores all previously generated terms in a dynamically
 *  sized array and checks membership by linear search.  The sequence is
 *  built lazily until the required index n is reached, so calls with
 *  non‑monotonic indices are handled correctly.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- dynamic array of seq_t used to store all terms ---------- */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

/* initialise the array if necessary */
static void terms_init(void)
{
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }
}

/* append a new term to the array */
static void terms_append(seq_t v)
{
    if (!terms) {
        terms_init();
    }
    if (terms_len == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = v;
}

/* linear search to test whether v has already appeared */
static int term_exists(seq_t v)
{
    for (size_t i = 0; i < terms_len; ++i) {
        if (terms[i] == v) {
            return 1;
        }
    }
    return 0;
}

/* total number of digits consumed so far in the alternating 1‑2 word */
static idx_t current_pos = 0;

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) /* sequence is 1‑indexed; bad request -> 0 */
        return 0;

    /* compute terms up to n if not already available */
    while (terms_len < n) {
        /* determine the digit at the current position in the 1‑2 word */
        uint8_t start_digit = (current_pos & 1ULL) ? 2 : 1;   /* 0→1, 1→2 */
        seq_t cur = start_digit;
        uint8_t next_digit = 3 - start_digit;
        size_t len = 1;

        /* extend until the number is unused */
        while (term_exists(cur)) {
            cur = cur * 10 + next_digit;
            next_digit = 3 - next_digit;
            ++len;
        }

        /* store the generated term and advance the position */
        terms_append(cur);
        current_pos += (idx_t)len;
    }

    return terms[n - 1];
}
