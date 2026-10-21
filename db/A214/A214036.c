/*
 * A214036  Numbers k such that floor(sqrt(1)) + floor(sqrt(2)) +
 *          floor(sqrt(3)) + ... + floor(sqrt(k)) is prime.
 * https://oeis.org/A214036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = k-th integer k such that * S(k) = Σ_{i=1}^k ⌊√i⌋ is
 *            prime. * The function below generates terms on demand, caching
 *            them in a * static array so that calls in any order are
 *            efficient
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, NULL, size_t */
#include <stdint.h>     /* uint64_t */
#include <stdbool.h>    /* bool */
#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64 */
#include "../utils/is_prime_big.h"       /* is_prime_big */

static idx_t   last_k   = 0;      /* last k processed                               */
static seq_t   sum      = 0;      /* current value of Σ_{i=1}^{last_k} ⌊√i⌋          */
static idx_t  *values   = NULL;   /* array of k values where the sum is prime         */
static size_t  val_len  = 0;      /* number of values stored                          */
static size_t  val_cap  = 0;      /* capacity of the values array                    */

/* grow the values array if needed */
static void
grow_values(void)
{
    if (val_len + 1 > val_cap) {
        size_t new_cap = (val_cap == 0) ? 16 : val_cap << 1;
        idx_t *p = (idx_t *)realloc(values, new_cap * sizeof(idx_t));
        if (p == NULL) {
            /* allocation failure – terminate the program */
            abort();
        }
        values = p;
        val_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Transform the 1‑based index to a 0‑based index for the array */
    idx_t target_index = n - 1;
    if (n == 0) {
        return 0;          /* undefined, but safe fallback */
    }

    /* Generate terms until we have at least n of them */
    while (val_len <= target_index) {
        /* Advance to the next k */
        last_k++;
        /* Add ⌊√last_k⌋ to the running sum */
        uint64_t inc = (uint64_t)_floor_isqrt_u64(last_k);
        sum += (seq_t)inc;

        /* Check if the current sum is a prime */
        if (is_prime_big(sum)) {
            grow_values();
            values[val_len++] = last_k;
        }
    }

    return values[target_index];
}
