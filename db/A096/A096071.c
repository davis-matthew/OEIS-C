/*
 * A096071  Primes arising as a partial sum of A096070.
 * https://oeis.org/A096071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096071 : primes arising as partial sums of A096070
   For n >= 1, a(n) = sum_{i=1}^{2n} a[i] where a[i] is the smallest unused
   positive integer such that if i is even the sum after adding a[i] is prime,
   otherwise the sum is not prime.  The sequence coincides with the partial
   sums of A096070 at the even indices. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1 */

    idx_t needed_terms = 2 * n;             /* total A096070 terms to generate */
    seq_t sum = 0;                          /* running sum of A096070 terms */

    /* dynamic array marking used integers; indices start at 1 */
    char *used = NULL;
    size_t used_size = 0;

    for (idx_t i = 1; i <= needed_terms; ++i)
    {
        bool want_prime = (i & 1) == 0;     /* even index => prime sum */

        idx_t cand = 1;                     /* smallest unused integer to test */
        for (;; ++cand)
        {
            /* grow used array if necessary */
            if (cand >= used_size)
            {
                size_t new_size = used_size ? used_size * 2 : 8;
                if (new_size <= cand) new_size = cand + 1;
                char *tmp = (char *)realloc(used, new_size);
                if (!tmp) { free(used); exit(EXIT_FAILURE); }
                memset(tmp + used_size, 0, new_size - used_size);
                used = tmp;
                used_size = new_size;
            }

            if (used[cand]) continue;        /* already used */

            seq_t new_sum = sum + (seq_t)cand;
            bool prime = is_prime_big(new_sum);

            if ((want_prime && prime) || (!want_prime && !prime))
            {
                /* candidate accepted */
                used[cand] = 1;
                sum = new_sum;
                break;
            }
        }
    }

    free(used);
    return sum;                              /* a(n) */
}
