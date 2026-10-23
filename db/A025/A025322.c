/*
 * A025322  Numbers that are the sum of 3 nonzero squares in exactly 2 ways.
 * https://oeis.org/A025322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer that can be expressed as the sum of three
 *            non‑zero squares in exactly two distinct unordered ways
 * Range:     u8 n <= 61, u16 n <= 418, u32 n <= 418, u64 n <= 418,
 *            u128 n <= 418, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a table of such numbers up to a chosen limit
   using a triple nested loop over non‑negative integers a ≤ b ≤ c,
   counting the representations for each sum. Consequently each call to a(n)
   constructs the table on first use or expands it when more terms are
   required.  The algorithm is O(L³) in the sieve limit L, which is
   sufficient for the required n ≈ 10 000 (a(10 000) < 10⁶).  Memory usage is
   bounded by the sieve limit. */

#include <stdint.h>
#include <stdlib.h>

/* Function prototype for external linkage (provided by the build system). */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Internal static state                                                   */
/* --------------------------------------------------------------------- */
static seq_t *terms   = NULL;   /* array of qualifying numbers             */
static size_t terms_cnt = 0;    /* number of items stored                 */
static uint64_t current_limit = 1000000;   /* default sieve limit              */

/* --------------------------------------------------------------------- */
/* Helper: builds a table of numbers with exactly two representations
   up to 'new_limit' (inclusive).  Old table is discarded.              */
/* --------------------------------------------------------------------- */
static void build_terms(uint64_t new_limit)
{
    if (new_limit == 0) return;

    /* Allocate a count array (uint16_t) for each possible sum. */
    uint16_t *cnt = (uint16_t*)calloc(new_limit + 1, sizeof(uint16_t));
    if (!cnt) abort();

    /* Determine maximum integer whose square ≤ new_limit. */
    uint64_t max_a = 0;
    while ((max_a + 1) * (max_a + 1) <= new_limit) ++max_a;

    /* Pre‑compute squares up to max_a. */
    uint64_t *sq = (uint64_t*)malloc((max_a + 1) * sizeof(uint64_t));
    if (!sq) { free(cnt); abort(); }
    sq[0] = 0;
    for (uint64_t i = 1; i <= max_a; ++i) sq[i] = i * i;

    /* Count unordered triples a ≤ b ≤ c. */
    for (uint64_t a = 1; a <= max_a; ++a) {
        uint64_t a2 = sq[a];
        for (uint64_t b = a; b <= max_a; ++b) {
            uint64_t ab = a2 + sq[b];
            if (ab > new_limit) break;
            for (uint64_t c = b; c <= max_a; ++c) {
                unsigned __int128 sum128 = (unsigned __int128)ab + sq[c];
                if (sum128 > new_limit) break;
                uint64_t sum = (uint64_t)sum128;
                if (cnt[sum] < 65535) ++cnt[sum];
            }
        }
    }
    free(sq);

    /* Collect numbers with exactly two representations. */
    size_t estimated = new_limit / 20 + 10;  /* rough lower bound            */
    seq_t *new_terms = (seq_t*)malloc(estimated * sizeof(seq_t));
    if (!new_terms) { free(cnt); abort(); }
    size_t sz = 0;
    for (uint64_t i = 1; i <= new_limit; ++i) {
        if (cnt[i] == 2) {
            if (sz >= estimated) {           /* grow if needed */
                estimated *= 2;
                new_terms = (seq_t*)realloc(new_terms, estimated * sizeof(seq_t));
                if (!new_terms) { free(cnt); abort(); }
            }
            new_terms[sz++] = (seq_t)i;
        }
    }
    free(cnt);

    /* Replace old table. */
    free(terms);
    terms = new_terms;
    terms_cnt = sz;
    current_limit = new_limit;
}

/* --------------------------------------------------------------------- */
/* Ensure we have at least 'idx' terms; expand the limit if necessary.   */
/* --------------------------------------------------------------------- */
static void ensure_terms(idx_t idx)
{
    if (terms_cnt >= (size_t)idx) return;         /* already have enough    */
    /* Build a larger table, doubling each step until satisfied.      */
    uint64_t new_limit = current_limit;
    while (terms_cnt < (size_t)idx) {
        new_limit *= 2;
        build_terms(new_limit);
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n<1     */
    /* Lazy construction. */
    if (!terms) build_terms(current_limit);
    ensure_terms(n);
    return terms[(size_t)(n - 1)];
}
