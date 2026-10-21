/*
 * A073542  Numbers k such that 1/(1/phi(k) + 1/phi(k+1)) is an integer.
 * https://oeis.org/A073542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th integer k such that φ(k)·φ(k+1) is divisible by
 *            φ(k)+φ(k+1)
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 1891, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *terms    = NULL;   /* array of computed terms */
static idx_t  cap      = 0;      /* current capacity */
static idx_t  len      = 0;      /* number of terms stored */
static idx_t  next_k   = 1;      /* next k to test */

/* extend the terms array when it runs out of space */
static void ensure_capacity(void)
{
    if (len >= cap) {
        idx_t new_cap = (cap == 0) ? 16 : cap << 1;
        seq_t *tmp = seq_realloc(terms, cap, new_cap);
        terms = tmp;
        cap   = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; terms below are undefined */

    /* generate terms until we reach the desired index */
    while (len < n) {
        seq_t phi_k  = phi(next_k);
        seq_t phi_k1 = phi(next_k + 1);
        seq_t prod   = phi_k * phi_k1;
        seq_t sum    = phi_k + phi_k1;

        if ((prod % sum) == 0) {
            ensure_capacity();
            terms[len++] = (seq_t)next_k;
        }
        next_k++;
    }
    return terms[n - 1];
}
