/*
 * A120187  a(1)=2; a(n) = floor((15 + Sum_{j=1..n-1} a(j))/7).
 * https://oeis.org/A120187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; a(n)=floor((15 + sum_{j=1}^{n-1} a(j))/7)
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  // for size_t, may be required by the build

#define MAX_N 20000  /* enough for the required test range */

static seq_t values[MAX_N];
static seq_t prefix[MAX_N];
static idx_t size = 0;   /* number of terms currently computed */

/* Compute the nth term of OEIS sequence A120187 */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined below offset, return 0 to avoid UB */
        return (seq_t)0;
    }

    /* initialize with the first term if necessary */
    if (size == 0) {
        values[0] = (seq_t)2;   /* a(1) */
        prefix[0] = (seq_t)2;   /* cumulative sum up to a(1) */
        size = 1;
    }

    /* grow the cache up to n */
    if (size < n) {
        /* guard against overflow of static array bounds */
        if (n > MAX_N) {
            /* For n beyond the static buffer, compute iteratively without caching */
            seq_t cum = (seq_t)0;
            seq_t result = (seq_t)0;
            for (idx_t k = 1; k <= n; ++k) {
                seq_t val = (((seq_t)15 + cum) / (seq_t)7);
                cum += val;
                if (k == n)
                    result = val;
            }
            return result;
        }

        for (idx_t k = size + 1; k <= n; ++k) {
            seq_t prev_sum = (k == 1) ? (seq_t)0 : prefix[k - 2];
            seq_t val = (((seq_t)15 + prev_sum) / (seq_t)7);
            values[k - 1] = val;
            if (k == 1)
                prefix[k - 1] = val;
            else
                prefix[k - 1] = prefix[k - 2] + val;
        }
        size = n;
    }

    return values[n - 1];
}
