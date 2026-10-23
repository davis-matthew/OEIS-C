/*
 * A135407  Partial products of A000032 (Lucas numbers beginning at 2).
 * https://oeis.org/A135407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=0..n} L(k) where L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2)
 *            (Lucas numbers)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Static cache for computed Lucas numbers and their prefix products. */
    static seq_t *lucas = NULL;
    static seq_t *prod  = NULL;
    static size_t cap   = 0;   /* allocated capacity of the arrays      */
    static size_t sz    = 0;   /* number of entries currently valid     */

    /* Grow the arrays if required.  Use 2× growth to keep amortized cost low. */
    if (n >= cap) {
        size_t new_cap = (cap == 0 ? 4 : cap * 2);
        while (n >= new_cap)
            new_cap *= 2;

        seq_t *new_lucas = seq_alloc_array(new_cap);
        seq_t *new_prod  = seq_alloc_array(new_cap);

        for (size_t i = 0; i < sz; ++i) {
            new_lucas[i] = lucas[i];
            new_prod[i]  = prod[i];
        }

        seq_free_array(lucas);
        seq_free_array(prod);

        lucas = new_lucas;
        prod  = new_prod;
        cap   = new_cap;
    }

    /* Initialise the first two Lucas numbers and products if we haven't. */
    if (sz == 0) {
        lucas[0] = 2;
        prod[0]  = lucas[0];
        sz = 1;
    }
    if (sz == 1) {
        lucas[1] = 1;
        prod[1]  = seq_mul(prod[0], lucas[1]);
        sz = 2;
    }

    /* Compute any missing Lucas numbers and prefix products up to index n. */
    while (sz <= n) {
        lucas[sz] = lucas[sz - 1] + lucas[sz - 2];
        prod[sz]  = seq_mul(prod[sz - 1], lucas[sz]);
        ++sz;
    }

    return prod[n];
}
