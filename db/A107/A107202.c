/*
 * A107202  Primes of the form x^2 + 88y^2.
 * https://oeis.org/A107202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p for which p ≡ 1, 9, 25, 49 or 81 (mod 88)
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime, matches_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This routine generates primes that satisfy the residue constraints
 *  and caches them so that any call to a(n) is amortised O(1)
 *  after the first computation.

 *  The implementation uses a simple odd‑candidate loop and the
 *  is_prime routine supplied in utils/is_prime.h.  All indices and
 *  values are handled in terms of the external types idx_t and
 *  seq_t supplied by the build.  For the caching array we store
 *  the prime values as uint64_t; this is safe because the 10 000th
 *  such prime is far below 2^64.  When returning a value we cast
 *  the stored uint64_t to seq_t, which works for ordinary
 *  integer types and for the arbitrary‑precision class that is
 *  used in some builds. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* provides bool is_prime(uint64_t) */
#include "../utils/matches_residue.h"

/*  Expand the dynamic array of uint64_t primes if needed.  */
static bool ensure_capacity(size_t needed, uint64_t **arr, size_t *cap)
{
    if (needed <= *cap) return true;
    size_t newcap = (*cap) ? (*cap) << 1 : 16;
    if (newcap < needed) newcap = needed;
    uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
    if (!tmp) return false;           /* allocation failed */
    *arr = tmp;
    *cap = newcap;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined at n=0 – return 0 */

    /* static cache of found primes */
    static uint64_t *buf  = NULL;          /* array of uint64_t primes */
    static size_t buf_cap = 0;             /* allocated capacity */
    static size_t buf_len = 0;             /* number of filled entries */
    static uint64_t last_checked = 2;      /* last candidate examined */

    /* Return cached value if already computed */
    if (n <= (idx_t)buf_len) {
        return (seq_t)buf[n - 1];
    }

    /* Allocate space for at least one more prime */
    if (!ensure_capacity(buf_len + 1, &buf, &buf_cap))
        return (seq_t)0;           /* memory allocation failure */

    /* Begin searching with the next odd candidate */
    uint64_t cand = last_checked + 1;
    if (cand <= 3) cand = 3;
    if ((cand & 1ULL) == 0) cand++;      /* ensure odd */

    /* Find primes until we have the nth term */
    while (buf_len < (size_t)n) {
        if (is_prime(cand) && matches_residue(cand)) {
            buf[buf_len++] = cand;
        }
        cand += 2;                      /* skip even numbers */
    }

    /* The last tested candidate was cand - 2 (since loop increases cand) */
    last_checked = cand - 2;

    return (seq_t)buf[n - 1];
}
