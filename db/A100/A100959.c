/*
 * A100959  Non-semiprimes.
 * https://oeis.org/A100959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that is NOT a semiprime (i.e., the
 *            number does not have exactly two prime factors, counting
 *            multiplicities)
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_semiprime_uint64.h"
#include "../utils/ensure_capacity.h"

/* Return the n-th non‑semiprime (a(1)=1, a(2)=2, …).  The function
   builds a static array of the found values on the first call and
   extends it lazily on subsequent calls, so each value is computed
   only once. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *non_semiprimes = NULL;  /* array of found values   */
    static idx_t   count   = 0;           /* number of values stored */
    static idx_t   next    = 1;           /* next integer to test    */
    static size_t  cap     = 0;           /* current array capacity  */

    if (n == 0)                     /* offset of sequence is 1 */
        return 0;

    /* generate values until we have at least n of them */
    while (count < n) {
        uint64_t v = next;          /* candidate integer       */
        if (!is_semiprime_uint64(v)) { /* not a semiprime */
            /* ensure we have enough room in the array */
            ensure_capacity(count + 1, &non_semiprimes, &cap);
            non_semiprimes[count] = (seq_t)v;
            ++count;
        }
        ++next;
    }
    return non_semiprimes[n - 1];
}
