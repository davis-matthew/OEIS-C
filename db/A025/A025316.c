/*
 * A025316  Numbers that are the sum of 2 distinct nonzero squares in 6 or
 *          more ways.
 * https://oeis.org/A025316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are the sum of two distinct non‑zero perfect squares
   in at least six distinct unordered ways.  The function is obtained by
   sieving all pairs a<b of positive integers, counting the number of
   representations for each sum, and collecting those sums whose count
   is >= 6.  The list is constructed only once and reused for all
   subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Helper to compute integer floor of sqrt(n).  Works for all n that
   fit in size_t.  The result is always <= sqrt(n). */
static size_t isqrt_size_t(size_t n)
{
    size_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* --------------------------------------------------------------------- */
/* Memoised storage for the sequence. */
static struct {
    seq_t *terms;     /* array of terms a(k) */
    size_t len;       /* number of terms currently stored */
    size_t cap;       /* allocated capacity */
    bool ready;       /* true when the list has been built */
} seq_state = {NULL, 0, 0, false};

/* --------------------------------------------------------------- */
/* Build the sequence up to a pre‑chosen limit that contains at
   least the first ~10 000 terms.  The limit is set to 20 000 000
   which comfortably covers this requirement. */
static void build_terms(void)
{
    const size_t LIMIT = 20000000U;      /* upper bound for sums */

    /* Count array: counts unordered representations of each sum.
       We cap the count at 6, so uint8_t is sufficient.          */
    uint8_t *cnt = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!cnt) return;                     /* allocation failure */

    size_t max_root = isqrt_size_t(LIMIT);

    /* Enumerate unordered pairs a<b of positive integers. */
    for (size_t a = 1; a <= max_root; ++a) {
        const size_t a2 = a * a;
        for (size_t b = a + 1; ; ++b) {
            const size_t b2 = b * b;
            const size_t sum = a2 + b2;
            if (sum > LIMIT) break;
            if (cnt[sum] < 6)          /* cap at 6 to avoid overflow   */
                ++cnt[sum];
        }
    }

    /* Allocate initial capacity for the list of terms. */
    const size_t INIT_CAP = 16384U;
    seq_t *terms = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
    size_t cap   = INIT_CAP;
    size_t len   = 0;

    /* Collect all sums with at least six representations. */
    for (size_t s = 1; s <= LIMIT; ++s) {
        if (cnt[s] >= 6) {
            if (len == cap) {                  /* grow if necessary   */
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms) { free(cnt); return; }
            }
            terms[len++] = (seq_t)s;
        }
    }

    free(cnt);

    /* Store the built data in the global state. */
    seq_state.terms = terms;
    seq_state.len   = len;
    seq_state.cap   = cap;
    seq_state.ready = true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function: a(n) returns the n‑th term (1‑based index).
   The list is built on first invocation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) is undefined   */
    if (!seq_state.ready) build_terms();   /* build lazily         */

    /* In case the pre‑selected limit was insufficient (unlikely for
       the specified n range), simply return 0 for safety. */
    if ((size_t)n > seq_state.len) return 0;

    return seq_state.terms[n - 1];
}
