/*
 * A080640  a(1) = 3; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) which is consistent with the condition "n is a
 *          member of the sequence if and only if a(n) is divisible by 4".
 * https://oeis.org/A080640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080640 – constructive generation
 *
 * a(1) = 3
 * For n > 1, let S = { a(1), a(2), …, a(n‑1) }.
 * If n ∈ S then a(n) is the smallest integer > a(n‑1) that is divisible by 4,
 * otherwise it is the smallest integer > a(n‑1) that is NOT divisible by 4.
 *
 * The sequence is generated incrementally; the arguments to the function may
 * arrive in any order.  Only integer arithmetic is used; the code compiles
 * under C11 and C++17.  All dynamic memory allocations are cast so that the
 * code is valid in C++ as well as C.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* external integer types defined by the build system */
extern seq_t a(idx_t n);

/* Static cache of already computed terms */
static idx_t cache_n   = 0;        /* number of terms currently cached */
static seq_t *terms    = NULL;    /* array of cached terms */

/* Bitset of values that have already appeared in the sequence (membership set) */
static bool *seen      = NULL;
static idx_t seen_size = 0;        /* allocated size of the seen array */

/* Ensure the seen array can be indexed up to val (inclusive). */
static void ensure_seen_size(seq_t val)
{
    idx_t idx = (idx_t)val;   /* values comfortably fit into idx_t for the test cases */
    if (idx < seen_size)
        return;

    size_t new_size = seen_size ? seen_size : 16;
    while (new_size <= (size_t)idx)
        new_size <<= 1;

    bool *tmp = (bool *)realloc(seen, new_size * sizeof(bool));
    if (!tmp)
        abort();                 /* allocation failure */

    /* initialise the newly allocated part with false */
    memset(tmp + seen_size, 0, (new_size - seen_size) * sizeof(bool));

    seen      = tmp;
    seen_size = (idx_t)new_size;
}

/* Ensure the terms array can hold at least N terms. */
static void ensure_terms_size(idx_t N)
{
    if (N <= cache_n)
        return;

    size_t new_size = cache_n ? cache_n : 1;
    while (new_size < (size_t)N)
        new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(terms, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure */

    terms = tmp;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for n < 1, guard */

    /* initialisation on first use */
    if (cache_n == 0) {
        /* allocate some room for terms and seen values */
        terms = (seq_t *)calloc(16, sizeof(seq_t));
        seen  = (bool *)calloc(16, sizeof(bool));
        if (!terms || !seen)
            abort();

        seen_size = 16;
        /* a(1) = 3 */
        terms[0] = (seq_t)3;
        ensure_seen_size(3);
        seen[(idx_t)3] = true;
        cache_n = 1;
    }

    /* extend cache if necessary */
    if (n > cache_n) {
        ensure_terms_size(n);

        seq_t prev = terms[cache_n - 1];

        for (idx_t i = cache_n; i < n; ++i) {
            idx_t idx = i + 1;           /* current index (1‑based) */
            bool isMember = (idx < seen_size) && seen[idx];

            seq_t candidate = prev + 1;
            bool divisible  = ((candidate & (seq_t)3) == 0);
            while (divisible != isMember) {
                ++candidate;
                divisible = ((candidate & (seq_t)3) == 0);
            }

            terms[i] = candidate;
            prev     = candidate;

            ensure_seen_size(candidate);
            seen[(idx_t)candidate] = true;
        }

        cache_n = n;
    }

    return terms[n - 1];
}
