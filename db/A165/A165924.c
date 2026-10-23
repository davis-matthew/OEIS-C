/*
 * A165924  a(n) = number of squareful nonzero quadratic residues of n.
 * https://oeis.org/A165924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_squareful_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A165924: a(n) = number of squareful nonzero quadratic residues modulo n
 * A squareful integer is one that is divisible by the square of a prime.
 * The function caches the list of such numbers up to the largest query seen
 * and uses a boolean array to record which residues appear as squares mod n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_is_squareful_u64.h"

static idx_t *squareful_arr = NULL;   /* list of squareful numbers found so far */
static size_t sf_count = 0;            /* number of elements in squareful_arr  */
static size_t sf_cap = 0;              /* capacity of squareful_arr            */
static idx_t sf_max = 0;               /* largest N up to which we have filled */

/* Fill squareful_arr with all squareful numbers ≤ N, extending the cache as needed.
 * The function is idempotent: calling it with an N ≤ sf_max does nothing. */
static void _ensure_squareful_up_to(idx_t N)
{
    if (N <= sf_max) return;

    /* Grow capacity to at least N (in steps of 2x to keep amortized cost). */
    if (sf_cap == 0) {
        sf_cap = 128;
        squareful_arr = (idx_t *)malloc(sf_cap * sizeof(*squareful_arr));
    }
    while (sf_cap <= N) {
        sf_cap <<= 1;
        squareful_arr = (idx_t *)realloc(squareful_arr,
                                         sf_cap * sizeof(*squareful_arr));
    }

    for (idx_t v = sf_max + 1; v <= N; ++v) {
        if (_is_squareful_u64(v)) {
            if (sf_count == sf_cap) {
                sf_cap <<= 1;
                squareful_arr = (idx_t *)realloc(squareful_arr,
                                                sf_cap * sizeof(*squareful_arr));
            }
            squareful_arr[sf_count++] = v;
        }
    }
    sf_max = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* defined only for n ≥ 1 */
    if (n > 1) _ensure_squareful_up_to(n - 1);

    /* Allocate a boolean array of size n to mark which residues occur. */
    bool *resid = (bool *)calloc((size_t)n, sizeof(*resid));
    if (resid == NULL) return 0;       /* allocation failure – return 0 */

    for (idx_t x = 0; x < n; ++x) {
        idx_t r = (x * x) % n;
        if (r != 0) resid[r] = true;   /* ignore the zero residue */
    }

    seq_t count = 0;
    for (size_t i = 0; i < sf_count; ++i) {
        idx_t v = squareful_arr[i];
        if (v >= n) break;            /* list sorted ascending */
        if (resid[v]) ++count;
    }

    free(resid);
    return count;
}
