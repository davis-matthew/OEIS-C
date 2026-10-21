/*
 * A076759  2-apexes of Omega: numbers k such that Omega(k-2)< Omega(k-1) <
 *          Omega(k) > Omega(k+1) > Omega(k+2), where Omega(m) = the number of
 *          prime factors of m, counting multiplicity.
 * https://oeis.org/A076759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076759 – 2‑apexes of the Omega function.
 *
 * An integer k is a 2‑apex of Omega if
 *     Ω(k-2) < Ω(k-1) < Ω(k) > Ω(k+1) > Ω(k+2)
 * where Ω(n) counts prime factors with multiplicity.
 *
 * This implementation searches integers sequentially and memoises the
 * discovered 2‑apexes.  It uses the provided utility function
 * bigomega(idx_t n) to obtain the Ω values.
 *
 * The function is correct for any n ≥ 1, in any call order, and runs in
 * roughly O(n) time for the first n terms.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

/* Dynamically resized array holding the discovered 2‑apexes. */
static seq_t *apexes = NULL;   /* array of found apex values */
static size_t cap = 0;         /* allocated capacity */
static size_t count = 0;       /* number of stored apexes */

/* Ensure that the array can hold at least one more element. */
static void ensure_more(void)
{
    if (count < cap)
        return;
    size_t newcap = (cap == 0) ? 64 : cap * 2;
    seq_t *tmp = (seq_t *)realloc(apexes, newcap * sizeof(seq_t));
    if (!tmp)  /* out of memory – abort silently */
        return;
    apexes = tmp;
    cap = newcap;
}

/* Append a new apex to the dynamic array. */
static void append_apex(seq_t v)
{
    ensure_more();
    if (count < cap)            /* double‑check after realloc */
        apexes[count++] = v;
}

/* Scan forward from the current position until at least `target`
 * apexes are found.  The starting position is kept as a static
 * variable so that consecutive calls continue where the previous left off.
 */
static void grow_to(idx_t target)
{
    static idx_t k = 3;          /* first candidate that can be a 2‑apex */
    while (count < (size_t)target) {
        seq_t om2 = bigomega(k - 2);
        seq_t om1 = bigomega(k - 1);
        seq_t om  = bigomega(k);
        seq_t op1 = bigomega(k + 1);
        seq_t op2 = bigomega(k + 2);

        if (om2 < om1 && om1 < om && om > op1 && op1 > op2)
            append_apex(k);
        ++k;
    }
}

/* Public entry point – returns the n-th 2‑apex of Omega. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0; return 0 safely */
        return (seq_t)0;
    grow_to(n);
    return apexes[n - 1];
}
