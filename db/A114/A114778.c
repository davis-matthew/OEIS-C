/*
 * A114778  Cumulative product of triple factorial A007661.
 * https://oeis.org/A114778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{j=0..n} j!!! ; Recurrence: a(0)=1, a(1)=1,
 *            a(2)=2, a(n)=a(n-1)*n!!! (n≥3); Triple factorial: n!!! =
 *            n*(n-3)!!! with 0!!!=1, 1!!!=1, 2!!!=2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

static seq_t *trifact = NULL;
static seq_t *prod = NULL;
static idx_t trifact_size = 0;
static idx_t prod_size = 0;

seq_t a(idx_t n)
{
    /* Expand cached tables if necessary */
    if (prod == NULL || prod_size <= n) {
        idx_t new_size = n + 1;

        trifact = seq_realloc(trifact, new_size);
        prod    = seq_realloc(prod,    new_size);

        for (idx_t i = trifact_size; i <= n; ++i) {
            if (i == 0) {
                trifact[0] = 1;
                prod[0]    = 1;
            } else if (i == 1) {
                trifact[1] = 1;
                prod[1]    = prod[0] * trifact[1];
            } else if (i == 2) {
                trifact[2] = 2;
                prod[2]    = prod[1] * trifact[2];
            } else {
                trifact[i] = (seq_t)i * trifact[i - 3];
                prod[i]    = prod[i - 1] * trifact[i];
            }
        }
        trifact_size = prod_size = new_size;
    }
    return prod[n];
}
