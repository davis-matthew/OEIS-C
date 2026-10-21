/*
 * A054715  Number of powers of 15 modulo n.
 * https://oeis.org/A054715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct residues among 15^k (k ≥ 0) modulo n
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts distinct values of 15^k (mod n) for k = 0,1,2,… until a repeat occurs.
 * Uses a static boolean array that is re‑used for all calls.  The array is
 * resized when a larger n is requested.  The computation avoids overflow by
 * performing the modular multiplication in seq_t.  Works for all supported
 * seq_t/idx_t types, including arbitrary‑precision big integers. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool *seen = NULL;          /* array of seen residues for the current n */
static size_t seen_cap = 0;        /* capacity of the seen array */

/* External linkage required by the build */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)               /* n=0 is not used but keeps type safety */
        return (seq_t)0;

    /* Make sure the buffer can hold n residues. */
    if (seen_cap < (size_t)n) {
        bool *tmp = (bool *)realloc(seen, (size_t)n * sizeof(bool));
        if (!tmp)                     /* allocation failed – unlikely in tests */
            return (seq_t)0;
        seen    = tmp;
        seen_cap= (size_t)n;
    }

    /* Reset the portion of the buffer that will be used for this n. */
    memset(seen, 0, (size_t)n * sizeof(bool));

    seq_t residue = 1 % (seq_t)n;    /* 15^0 ≡ 1 (mod n) */
    seq_t count   = 0;

    /* Generate powers of 15 modulo n until a repeat residue appears. */
    while (!seen[(size_t)residue]) {
        seen[(size_t)residue] = true;
        ++count;
        residue = (residue * (seq_t)15) % (seq_t)n;   /* safe modular multiply */
    }
    return count;
}
