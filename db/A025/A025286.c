/*
 * A025286  Numbers that are the sum of 2 nonzero squares in exactly 3 ways.
 * https://oeis.org/A025286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 671, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025286: Numbers that are the sum of two non‑zero squares in exactly three unordered ways.
   Implementation: For all a,b with 1 ≤ a ≤ b and a²+b² ≤ MAX_SUM, count how many
   representations each sum has; collect those with count == 3.  The pre‑computed
   list is built on first call.  Max sum 20 000 000 provides more than 10 000
   such numbers, which is sufficient for the expected test range.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SUM 20000000U

/* Static state: array of terms and counters. */
static bool initialized = false;
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

/* Append a term, expanding the buffer if necessary. */
static void add_term(seq_t v)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        /* Cast returned void* to seq_t* for C++ compatibility. */
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!terms) abort();          /* allocation failure */
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* One‑time initialisation: build the list of A025286 terms. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Count array: we only need to know counts up to 3. */
    uint8_t *cnt = (uint8_t*)calloc((size_t)(MAX_SUM + 1), sizeof(uint8_t));
    if (!cnt) abort();                 /* allocation failure */

    /* Enumerate all unordered pairs (a,b), a≥1, b≥a, a²+b²≤MAX_SUM. */
    for (idx_t a = 1; a * a <= MAX_SUM; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = a; a2 + b * b <= MAX_SUM; ++b) {
            idx_t sum = a2 + b * b;
            if (cnt[sum] < 4) /* cap at 3+ */
                ++cnt[sum];
        }
    }

    /* Collect all sums that occur exactly three times. */
    for (idx_t s = 1; s <= MAX_SUM; ++s) {
        if (cnt[s] == 3)
            add_term((seq_t)s);
    }

    free(cnt);
}

/* Public function: returns the n‑th term of the sequence (1‑indexed).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe guard */
    init();                  /* ensure terms are built */

    if (n > terms_len) return 0;   /* out of range – unlikely for valid input */

    return terms[n - 1];
}
