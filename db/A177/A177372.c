/*
 * A177372  Fibonacci numbers whose decimal expansion does not contain any
 *          digit 7.
 * https://oeis.org/A177372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   fib, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: Fibonacci numbers whose decimal expansion does not contain any digit 7.
   Computed by searching the Fibonacci sequence up to index 224 (the largest
   Fibonacci without a 7), memoising the results. */

#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* Off‑set 1: a(1) corresponds to the first such Fibonacci number. */
    if (n == 0)
        return 0;   /* undefined for n < 1 */

    /* Static storage for memoised results */
    static seq_t cache[1024];            /* enough for all admissible terms */
    static size_t count = 0;             /* number of terms stored in cache   */
    static idx_t next_fib_idx = 1;       /* next Fibonacci index to test      */

    /* Keep generating until we have at least n terms or we run past index 224 */
    while (count < n && next_fib_idx <= 224) {
        seq_t f = fib(next_fib_idx++);    /* compute Fibonacci number */

        /* Extract decimal digits into a small buffer */
        uint8_t digits[200];
        size_t len;
        get_digits(f, digits, &len);

        /* Check that none of the digits is 7 */
        bool ok = true;
        for (size_t i = 0; i < len; ++i) {
            if (digits[i] == 7) {
                ok = false;
                break;
            }
        }
        if (ok) {
            cache[count++] = f;           /* store the qualifying term   */
        }
    }

    /* If the requested index exceeds the number of terms found,
       return 0 (sequence ends beyond this point). */
    if (n - 1 >= count)
        return 0;

    return cache[n - 1];
}
