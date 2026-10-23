/*
 * A154234  Triangle read by rows: T(n,k) = (n mod d(k)), where d(i) is the
 *          number of divisors of i.
 * https://oeis.org/A154234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = n mod d(k) (d(k) = number of divisors of k). The
 *            sequence a(n) is the triangle read by rows: a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   compute_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cache of divisor counts up to the
   largest column requested.  It works for any seq_t and idx_t
   supplied by the build. */

#include <stdlib.h>   /* for realloc, abort */
#include "../utils/compute_divisor_count.h"

/* Divisor‑count cache (1‑based).  divcnt[i] = d(i). */
static uint32_t *divcnt = NULL;
static idx_t divcnt_size = 0;   /* highest index stored in divcnt */

/* Ensure that divcnt has entries up to 'limit'. */
static void ensure_divcnt(idx_t limit)
{
    if (limit <= divcnt_size) return;

    uint32_t *tmp = (uint32_t *)realloc(divcnt, (limit + 1) * sizeof(uint32_t));
    if (!tmp) abort();          /* unrecoverable memory error */
    divcnt = tmp;

    for (idx_t i = divcnt_size + 1; i <= limit; ++i) {
        divcnt[i] = compute_divisor_count(i);
    }
    divcnt_size = limit;
}

/* Main entry point required by the boilerplate. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* defensive: offset starts at 1 */

    /* Find row 'r' so that (r-1)*r/2 < n <= r*(r+1)/2. */
    idx_t r = 1;
    while (((seq_t)r * ((seq_t)r + 1) / 2) < n) ++r;

    /* Column index 'k' within this row (1‑based). */
    idx_t prevTri = (r - 1) * r / 2;
    idx_t k = n - prevTri;            /* 1 ≤ k ≤ r */

    /* Make sure divisor counts up to column r exist. */
    ensure_divcnt(r);

    seq_t result = (seq_t)r % (seq_t)divcnt[k];
    return result;
}
